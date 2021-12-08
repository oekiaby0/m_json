#ifndef L_LIST_H
#define L_LIST_H

#include <stddef.h>

typedef struct l_node {
    struct l_node* prev;
    struct l_node* next;
    void* data;
} l_node;

typedef struct l_list {
      l_node* end;
      l_node* start;
      size_t length;
} l_list;

l_node* l_node_create(void* data);
l_list* l_list_create();

void l_list_free(l_list* list);
void l_list_free_data(l_list* list);

void l_list_push_node(l_list* list, l_node* node);
void l_list_push(l_list* list, void* data);

void l_list_prepend_node(l_list* list, l_node* target, l_node* node);
void l_list_prepend(l_list* list, l_node* target, void* data);

l_node* l_list_pop_node(l_list* list, size_t index);
void* l_list_pop(l_list* list, size_t index);
l_node* l_list_pop_last_node(l_list* list);
void* l_list_pop_last(l_list* list);

l_node* l_list_get_index_node(l_list* list, size_t index);
void* l_list_get_index(l_list* list, size_t index);

#endif