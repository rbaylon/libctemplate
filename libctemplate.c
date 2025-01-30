#include "libctemplate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* read_file(const char* filepath) {
    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* content = (char*)malloc(length + 1);
    if (content == NULL) {
        fclose(file);
        return NULL;
    }

    size_t read_size = fread(content, 1, length, file);
    if (read_size != length) {
        free(content);
        fclose(file);
        return NULL;
    }
    content[length] = '\0';

    fclose(file);
    return content;
}

void replace_placeholder(char* template, GetValueCallback get_value, void* data) {
    size_t template_len = strlen(template);
    size_t buffer_size = template_len + 1;
    char* buffer = (char*)malloc(buffer_size);
    if (buffer == NULL) {
        return;
    }
    strncpy(buffer, template, buffer_size);

    char* pos;
    while ((pos = strstr(buffer, "{{")) != NULL) {
        char* end = strstr(pos, "}}");
        if (end == NULL) {
            break;
        }

        size_t key_len = end - pos - 2;
        char* key = (char*)malloc(key_len + 1);
        if (key == NULL) {
            free(buffer);
            return;
        }
        strncpy(key, pos + 2, key_len);
        key[key_len] = '\0';

        const char* value = get_value(data, key);
        size_t val_len = strlen(value);
        size_t placeholder_len = key_len + 4; // {{key}}

        size_t new_buffer_size = buffer_size + val_len - placeholder_len;
        char* new_buffer = (char*)realloc(buffer, new_buffer_size);
        if (new_buffer == NULL) {
            free(key);
            free(buffer);
            return;
        }
        buffer = new_buffer;
        buffer_size = new_buffer_size;

        memmove(pos + val_len, pos + placeholder_len, strlen(pos) - placeholder_len + 1);
        memcpy(pos, value, val_len);

        free(key);
    }

    strncpy(template, buffer, buffer_size);
    free(buffer);
}

void render_template(const char* template_path, GetValueCallback get_value, void* data) {
    char* template_content = read_file(template_path);
    if (template_content == NULL) {
        fprintf(stderr, "Error: Unable to read template file\n");
        return;
    }

    replace_placeholder(template_content, get_value, data);
    printf("%s\n", template_content);

    free(template_content);
}

const char* get_value(void* data, const char* key) {
    KeyValueList* kv_list = (KeyValueList*)data;
    for (size_t i = 0; i < kv_list->count; ++i) {
        if (strcmp(kv_list->pairs[i].key, key) == 0) {
            return kv_list->pairs[i].value;
        }
    }
    return "";
}

