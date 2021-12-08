#ifndef LEXER_H
#define LEXER_H

#include "l_list.h"

#define IS_WHITESPACE(c) (c == '\t' || c == '\r' || c == '\f' || c == '\n' || c == ' ')
#define IS_JSON_CHAR(c) (c == '{' || c == '}' || c == '[' || c == ']' || c == ':' || c == ',' || c == '"')
#define IS_LETTER(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
#define IS_NUMBER(c) (c >= '0' && c <= '9')

enum json_token_type {
    JSON_NULL,
    JSON_TRUE,
    JSON_FALSE,
    JSON_STRING,
    JSON_NUMBER,
    JSON_OPEN_BRACKETS,
    JSON_CLOSE_BRACKETS,
    JSON_OPEN_BRACES,
    JSON_CLOSE_BRACES,
    JSON_QUOTE,
    JSON_COLON,
    JSON_COMMA
};

typedef struct m_json_token {
    enum json_token_type type;
    char* str;
    double number;
    char* c;
} m_json_token;

l_list* lexer(char* c);
void lexer_tokens_free(l_list* tokens);

#endif