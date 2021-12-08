#include "m_json.h"
#include "lexer.h"
#include "sbuilder.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_BUFFER_LENGTH 22

m_json* m_json_create() {
    m_json* json = malloc(sizeof(m_json));
    json->prev = NULL;
    json->next = NULL;
    json->child = NULL;
    json->key = NULL;
    json->string = NULL;
    json->data_type = 0;
    json->type = 0;
    return json;
}

void m_json_free(m_json* root) {
    if (!root) return;
    if (root->child) {
        m_json_free(root->child);
    }
    if (root->next) {
        m_json_free(root->next);
    }
    if (root->data_type == m_json_STRING) {
        free(root->string);
    }
    free(root);
}

void m_json_push_child(m_json* json, m_json* child) {
    m_json* last_child = json->child;
    if (last_child) {
        while (last_child->next) last_child = last_child->next;
        last_child->next = child;
        child->prev = last_child;
    } else {
        json->child = child;
    }
}

int parse_value(m_json* json, m_json_token* token) {
    switch (token->type) {
        case JSON_NULL: json->data_type = m_json_NULL; break;
        case JSON_TRUE: json->data_type = m_json_TRUE; break;
        case JSON_FALSE: json->data_type = m_json_FALSE; break;
        case JSON_STRING: {
            json->data_type = m_json_STRING;
            json->string = token->str;
            break;
        }
        case JSON_NUMBER: {
            json->data_type = m_json_NUMBER;
            json->number = token->number;
            break;
        }
        default: {
            return 0;
        }
    }
    return 1;
}

m_json* parse_object(l_node** p_node);
m_json* parse_array(l_node** p_node);

# define FAIL_PARSE_ARRAY(){\
    m_json_free(array);\
    return NULL;\
}

m_json* parse_array(l_node** p_node) {
    if (!p_node || !*p_node) return NULL;
    l_node* node = (*p_node);
    m_json_token* token = node->data;
    node = node->next;
    if (!node || !token || token->type != JSON_OPEN_BRACKETS) return NULL;
    token = node->data;
    if (!token) return NULL;

    m_json* array = m_json_create();
    array->type = m_json_ARRAY;
    array->data_type = m_json_ARRAY_ITEM;
    while (token && token->type != JSON_CLOSE_BRACKETS) {
        m_json* item;
        switch (token->type) {
            case JSON_OPEN_BRACES: {
                item = parse_object(&node);
                break;
            }
            case JSON_OPEN_BRACKETS: {
                item = parse_array(&node);
                break;
            }   
            default: {
                item = m_json_create();
                if (!parse_value(item, token)) {
                    m_json_free(item);
                    FAIL_PARSE_ARRAY();
                }
            }
        }
        if (!item) FAIL_PARSE_ARRAY();
        item->type = m_json_ARRAY_ITEM;
        m_json_push_child(array, item);


        node = node->next;
        if (!node) FAIL_PARSE_ARRAY();
        token = node->data;
        if (!token) FAIL_PARSE_ARRAY();
        if (token->type == JSON_CLOSE_BRACKETS) break;
        if (token->type != JSON_COMMA) FAIL_PARSE_ARRAY();
        node = node->next;
        if (!node) FAIL_PARSE_ARRAY();
        token = node->data;
        if (!token) FAIL_PARSE_ARRAY();
    }
    *p_node = node;
    return array;
}

# define FAIL_PARSE_OBJECT(){\
    m_json_free(object);\
    return NULL;\
}

m_json* parse_object(l_node** p_node) {
    if (!p_node || !*p_node) return NULL;
    l_node* node = (*p_node);
    m_json_token* token = node->data;
    node = node->next;
    if (!node || !token || token->type != JSON_OPEN_BRACES) return NULL;
    token = node->data;
    if (!token) return NULL;

    m_json* object = m_json_create();
    object->type = m_json_OBJECT;
    object->data_type = m_json_OBJECT_ITEM;

    while (token && token->type != JSON_CLOSE_BRACES) {
        if (token->type != JSON_STRING) FAIL_PARSE_OBJECT();
        char* key = token->str;
        node = node->next;
        if (!node) FAIL_PARSE_OBJECT();
        token = node->data;
        if (!token || token->type != JSON_COLON) FAIL_PARSE_OBJECT();

        node = node->next;
        if (!node) FAIL_PARSE_OBJECT();
        token = node->data;
        if (!token) FAIL_PARSE_OBJECT();
        m_json* item;
        switch (token->type) {
            case JSON_OPEN_BRACES: {
                item = parse_object(&node);
                break;
            }
            case JSON_OPEN_BRACKETS: {
                item = parse_array(&node);
                break;
            }   
            default: {
                item = m_json_create();
                if (!parse_value(item, token)) {
                    m_json_free(item);
                    FAIL_PARSE_OBJECT();
                }
            }
        }
        if (!item) FAIL_PARSE_OBJECT();
        item->key = key;
        item->type = m_json_OBJECT_ITEM;
        m_json_push_child(object, item);

        
        node = node->next;
        if (!node) FAIL_PARSE_OBJECT();
        token = node->data;
        if (!token) FAIL_PARSE_OBJECT();
        if (token->type == JSON_CLOSE_BRACES) break;
        if (token->type != JSON_COMMA) FAIL_PARSE_OBJECT();
        
        node = node->next;
        if (!node) FAIL_PARSE_OBJECT();
        token = node->data;
        if (!token) FAIL_PARSE_OBJECT();
    }
    *p_node = node;
    return object;
}

m_json* m_json_parse_object(char* json) {
    l_list* tokens = lexer(json);
    if (tokens == NULL) return NULL;
    l_node* start = tokens->start;
    m_json* json_object = parse_object(&tokens->start);
    tokens->start = start;
    l_list_free(tokens);
    return json_object;
}

m_json* m_json_parse_array(char* json) {
    l_list* tokens = lexer(json);
    if (tokens == NULL) return NULL;
    l_node* start = tokens->start;
    m_json* json_array = parse_array(&tokens->start);
    tokens->start = start;
    l_list_free(tokens);
    return json_array;
}

m_json* m_json_parse(char* json) {
    l_list* tokens = lexer(json);
    if (tokens == NULL) return NULL;
    l_node* start = tokens->start;
    m_json* json_struct = NULL;
    if (((m_json_token*) tokens->start->data)->type == JSON_OPEN_BRACKETS) {
        json_struct = parse_array(&tokens->start);
    } else if (((m_json_token*) tokens->start->data)->type == JSON_OPEN_BRACES) {
        json_struct = parse_object(&tokens->start);
    } else {
        l_list_free(tokens);
        return NULL;
    }
    tokens->start = start;
    l_list_free(tokens);
    return json_struct;
}

// TODO: this is VERY inefficient.
#define SB_APPEND(str) { \
    char* c = malloc(strlen(str)+1);\
    strcpy(c, str);\
    sb_append(sb, c);\
}

void json_string_recursive(m_json* head, char* delimiter, sbuilder* sb) {
    if (head->data_type == m_json_OBJECT_ITEM || head->data_type == m_json_ARRAY_ITEM) {
        if (head->key) {
            SB_APPEND("\"");
            SB_APPEND(head->key);
            SB_APPEND("\":");
        }
        SB_APPEND( head->data_type == m_json_OBJECT_ITEM ? "{" : "[");
        if (head->child) {
            json_string_recursive(head->child, ",", sb);
        }
        SB_APPEND( head->data_type == m_json_OBJECT_ITEM ? "}" : "]");
        if (head->next) {
            if (delimiter) {
                SB_APPEND( delimiter);
            }
            json_string_recursive(head->next, delimiter, sb);
        }
    } else {
        char* str = NULL;
        double num;
        switch (head->data_type) {
            case m_json_STRING: str = head->string; break;
            case m_json_TRUE: str = "true"; break;
            case m_json_FALSE: str = "false"; break;
            case m_json_NULL: str = "null"; break;
            case m_json_NUMBER: num = head->number; break;
            default: str = "?";
        }
        if (head->key) {
                SB_APPEND( "\"");
                SB_APPEND( head->key);
                SB_APPEND( "\":");
        }
        if (str) {
            if (head->data_type == m_json_STRING) {
                SB_APPEND( "\"");
                SB_APPEND( str);
                SB_APPEND( "\"");
            } else {
                SB_APPEND( str);
            }
        } else {
            char* buffer = malloc(sizeof(char) * NUMBER_BUFFER_LENGTH);
            sprintf(buffer, "%.16g", num);
            sb_append(sb, buffer);
        }
        if (head->next) {
            SB_APPEND( delimiter);
        }

        if (head->child) {
            json_string_recursive(head->child, delimiter, sb);
        }
        if (head->next) {
            json_string_recursive(head->next, delimiter, sb);
        }
    }
}

char* m_json_to_str(m_json* json) {
    sbuilder* sb = sb_create();
    json_string_recursive(json, NULL, sb);
    char* str = sb_concat(sb);
    sb_free(sb);
    return str;
}
