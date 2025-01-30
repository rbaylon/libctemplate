#include "libctemplate.h"
#include <stdio.h>

int main() {
    KeyValuePair pairs[] = {
        {"name", "World"}
    };

    KeyValueList kv_list;
    kv_list.pairs = pairs;
    kv_list.count = sizeof(pairs) / sizeof(pairs[0]);

    render_template("template.txt", get_value, &kv_list);
    return 0;
}

