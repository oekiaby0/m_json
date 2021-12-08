#include <stddef.h>

typedef struct sbuilder {
    char* str;
    struct sbuilder* next;
    struct sbuilder* end;
} sbuilder;

sbuilder* sb_create();
void sb_free(sbuilder* sb);

void sb_append(sbuilder* sb, char* str);
size_t sb_length(sbuilder* sb);
char* sb_concat(sbuilder* sb);