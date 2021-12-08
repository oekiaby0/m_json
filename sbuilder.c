#include "sbuilder.h"

#include <stdlib.h>
#include <string.h>

sbuilder* sb_create() {
    sbuilder* sb = (sbuilder*) malloc(sizeof(sbuilder));
    sb->str = NULL;
    sb->next = NULL;
    sb->end = NULL;
    return sb;
}

void sb_free(sbuilder* sb) {
    while (sb) {
        sbuilder* old = sb;
        sb = sb->next;
        if (old->str) free(old->str);
        free(old);
    }
}

void sb_append(sbuilder* sb, char* str) {
    if (!str) return;
    if (sb->str) {
        sbuilder* new_sb = sb_create();
        new_sb->str = str;
        if (sb->end) {
            sb->end->next = new_sb;
        } else {
            sb->next = new_sb;
        }
        sb->end = new_sb;
    } else {
        sb->str = str;
    }
}

size_t sb_length(sbuilder* sb) {
    size_t l = 0;
    for (; sb; sb = sb->next) l+=strlen(sb->str);
    return l;
}

char* sb_concat(sbuilder* sb) {
    char* str = malloc(sb_length(sb) + 1);
    char* start = str;
    for (; sb; sb = sb->next) {
        for (char* c = sb->str; *c; c++) {
            *str++ = *c;
        }
    }
    *str = '\0';
    return start;
}
