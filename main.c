#include <stdio.h>
#include "m_json.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Invalid amount of arguments.\n");
        return 1;
    }
    m_json* json = m_json_parse(argv[1]);
    if (!json) {
        fprintf(stderr, "Invalid JSON.\n");
        return 1;
    }
    char* json_str = m_json_to_str(json);
    printf("%s\n", json_str);
    free(json_str);
    m_json_free(json);
    return 0;
}
