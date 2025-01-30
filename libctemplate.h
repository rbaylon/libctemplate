#include <stdlib.h>
#ifndef LIBCTEMPLATE_H
#define LIBCTEMPLATE_H 

typedef struct {
    const char* key;
    const char* value;
} KeyValuePair;

typedef struct {
    KeyValuePair* pairs;
    size_t count;
} KeyValueList;

typedef const char* (*GetValueCallback)(void* data, const char* key);

void render_template(const char* template_path, GetValueCallback get_value, void* data);
const char* get_value(void* data, const char* key);

#endif // LIBCTEMPLATE_H

