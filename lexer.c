#include "lexer.h"

#include <stdlib.h>
#include <string.h>

#define TOKEN_CREATE() ({\
    m_json_token* token = malloc(sizeof(m_json_token));\
    token->type = 0;\
    token->str = NULL;\
    token->c = NULL;\
    token;\
})

int str_has(char *array, char c) {
    while (*array) {
        if (c == *array) return 1;
        array++;
    }
    return 0;
}

char* get_str(char**c, char* terminate_chars) {
    char* s = *c;
    size_t len = 0;
    while(*s && !str_has(terminate_chars, *s)) {
        if (!IS_WHITESPACE(*s)) len++;
        s++;
    }
    if (!*s) return NULL;
    char* begin = *c;
    *c = s;
    char* str = malloc(sizeof(char) * (len+1));
    char* b = str;
    int char_found = 0;
    while (len) {
        if (!IS_WHITESPACE(*begin)) {
            char_found = 1;
            *(str++) = *begin++;
            len--;
        } else if (char_found) {
            free(b);
            return NULL;
        } else {
            begin++;
        }
    }
    *str = '\0';
    return b;
}

char* get_json_str(char**c) {
    char* s = *c;
    char last_escape = 0;
    size_t escape_count = 0;
    while (*s && (*s != '"' || last_escape == 1)) {
        if (last_escape) {
            last_escape = 0;
            escape_count++;
        } else if (*s == '\\') {
            last_escape = 1;
        }
        s++;
    }
    if (!*s) return NULL;
    size_t len = ((s - *c) - escape_count);
    char* begin = *c;
    *c = s;
    char* str = malloc(sizeof(char) * (len+1));
    char* b = str;
    while (len) {
        if (last_escape) {
            switch (*begin) {
                case '"': {
                    *str = '"';
                    break;
                }
                case '\\': {
                    *str = '\\';
                    break;
                }
                case '/': {
                    *str = '/';
                    break;
                }
                case 'b': {
                    *str = '\b';
                    break;
                }
                case 'f': {
                    *str = '\f';
                    break;
                }
                case 'n': {
                    *str = '\n';
                    break;
                }
                case 'r': {
                    *str = '\r';
                    break;
                }
                case 't': {
                    *str = '\t';
                    break;
                }
                case 'u': {
                    if (len > 4) {
                        *str = '\\';
                        *++str = 'u';
                        len--;
                    } else {
                        free(b);
                        return NULL;
                    }
                    break;
                }
                default: {
                    free(b);
                    return NULL;
                }
            }
            last_escape = 0;
        } else if (*begin == '\\')  {
            last_escape = 1;
            begin++;
            continue;
        } else {
            *str = *begin;
        }
        begin++;
        str++;
        len--;
    }
    *str = '\0';
    return b;
}

void lexer_tokens_free(l_list* tokens) {
    for (l_node* node = tokens->start; node; node = node->next) {
        m_json_token* token = node->data;
        if (token->str) {
            free(token->str);
        }
        free(token);
    }
    l_list_free(tokens);
}

m_json_token* lexer_keyword(char*c) {
    char terminate_characters[] = {',', '}', ']'};
    char* str = get_str(&c, terminate_characters);
    if (str == NULL) return NULL;
    m_json_token* token = TOKEN_CREATE();
    if (strcmp(str, "true") == 0) {
        token->type = JSON_TRUE;
    } else if (strcmp(str, "false") == 0) {
        token->type = JSON_FALSE;
    } else if (strcmp(str, "null") == 0) {
        token->type = JSON_NULL;
    } else {
        free(token);
        return NULL;
    }
    free(str);
    token->c = c;
    return token;
}

m_json_token* lexer_string(char* c) {
    if (!*c) return NULL;
    char* str = get_json_str(&c);
    if (str == NULL) return NULL;
    m_json_token* token = TOKEN_CREATE();
    token->type = JSON_STRING;
    token->str = str;
    token->c = ++c;
    return token;
}

m_json_token* lexer_number(char* c) {
    char terminate_characters[] = {',', '}', ']'};
    char* str = get_str(&c, terminate_characters);
    if (str == NULL) return NULL;
    char* err;
    double number = strtod(str, &err);
    if (*err) {
        free(str);
        return NULL;
    };
    free(str);
    m_json_token* token = TOKEN_CREATE();
    token->type = JSON_NUMBER;
    token->number = number;
    token-> c = c;
    return token;
}

m_json_token* lexer_char(char* c) {
    m_json_token* token = TOKEN_CREATE();
    token->c = c+1;
    switch(*c) {
        case '{': token->type = JSON_OPEN_BRACES; break;
        case '}': token->type = JSON_CLOSE_BRACES; break;
        case '[': token->type = JSON_OPEN_BRACKETS; break;
        case ']': token->type = JSON_CLOSE_BRACKETS; break;
        case '"': token->type = JSON_QUOTE; break;
        case ',': token->type = JSON_COMMA; break;
        case ':': token->type = JSON_COLON; break;
        default: {
            free(token);
            return NULL;
        }
    }
    return token;
}

l_list* lexer(char* c) {
    l_list* tokens = l_list_create();
    while (c && *c) {
        if (IS_WHITESPACE(*c)) {
            c++;
        } else {
            m_json_token* token = NULL;
            if (*c == '"') {
                token = lexer_string(c+1);
            } else if (IS_LETTER(*c)) {
                token = lexer_keyword(c);
            } else if (IS_NUMBER(*c) || *c == '-' || *c == '+') {
                token = lexer_number(c);
            } else if (IS_JSON_CHAR(*c)) {
                token = lexer_char(c);
            }
            if (token == NULL) {
                lexer_tokens_free(tokens);
                return NULL;
            }
            l_list_push(tokens, token);
            c = token->c;
            token->c = NULL;
        }
    };
    return tokens;
}
