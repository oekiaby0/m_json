#ifndef m_json_H
#define m_json_H

#include "l_list.h"

#include <stdlib.h>

enum m_json_type {
    m_json_NULL=1,
    m_json_OBJECT,
    m_json_OBJECT_ITEM,
    m_json_ARRAY,
    m_json_ARRAY_ITEM,
    m_json_STRING,
    m_json_NUMBER,
    m_json_TRUE,
    m_json_FALSE
};

typedef struct m_json {
    enum m_json_type type;
    enum m_json_type data_type;
    struct m_json* prev;
    struct m_json* next;
    char* key;
    struct m_json* child;
    char* string;
    double number;
} m_json;


m_json* m_json_parse_object(char* json);
m_json* m_json_parse_array(char* json);
m_json* m_json_parse(char* json);
void m_json_push_child(m_json* json, m_json* child);
m_json* m_json_create();
char* m_json_to_str(m_json* json);
void m_json_free(m_json* root);

#endif