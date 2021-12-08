#include "l_list.h"

#include <stdlib.h>

l_node* l_node_create(void* data) {
    l_node* node = malloc(sizeof(l_list));
    node->data = data;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

l_list* l_list_create() {
    l_list* list = malloc(sizeof(l_list));
    list->length = 0;
    list->start = NULL;
    list->end = NULL;
    return list;
}

void l_list_free(l_list* list) {
    l_node* node = list->end;
    while (node) {
        l_node* old = node;
        node = node->prev;
        free(old);
    }
}

void l_list_free_data(l_list* list) {
    l_node* node = list->end;
    while (node) {
        l_node* old = node;
        node= node->prev;
        free(old->data);
        free(old);
    }
}

l_node* l_list_get_index_node(l_list* list, size_t index) {
    l_node* node = list->start;
    while (--index) node = node->next;
    return node;
}

void* l_list_get_index(l_list* list, size_t index) {
    return l_list_get_index_node(list, index)->data;
}

void l_list_push_node(l_list* list, l_node* node) {
    node->next = NULL;
    node->prev = list->end;
    if (list->end) {
        list->end->next = node;
    }
    list->end = node;
    if (!list->start) list->start = node;
    list->length++;
}

void l_list_push(l_list* list, void* data) {
    l_list_push_node(list, l_node_create(data));
}

void l_list_prepend_node(l_list* list, l_node* target, l_node* node) {
    if (target->prev) {
        target->prev->next = node;
        node->prev = target->prev; 
    } else {
        list->start = node;
    }
    target->prev = node;
    node->next = target;
    list->length++;
}

void l_list_prepend(l_list* list, l_node* target, void* data) {
    l_list_prepend_node(list, target, l_node_create(data));
}

void l_list_append_node(l_list* list, l_node* target, l_node* node) {
    if (target->next) {
        target->next->prev = node;
        node->next = target->next;
    } else {
        list->end = node;
    }
    target->next = node;
    node->prev = target;
    list->length++;
}

void l_list_append(l_list* list, l_node* target, void* data) {
    l_list_append_node(list, target, l_node_create(data));
}

l_node* l_list_pop_node(l_list* list, size_t index) {
    l_node* node = l_list_get_index_node(list, index);

    if (list->end == node && node->prev) list->end = node->prev;
    if (list->start == node && node->next) list->start = node->next;
    if (node->next) node->next->prev = node->prev;
    if (node->prev) node->prev->next = node->next;

    node->prev = NULL;
    node->next = NULL;
    list->length--;
    return node;
}

void* l_list_pop(l_list* list, size_t index) {
    l_node* node = l_list_pop_node(list, index);
    void* data = node->data;
    free(node);
    return data;
}

l_node* l_list_pop_last_node(l_list* list) {
    l_node* node = list->end;
    if (node->prev) {
        list->end = node->prev;
    } else {
        list->end = NULL;
        list->start = NULL;
    }
    if (node->prev) node->prev->next = NULL;
    node->prev = NULL;
    node->next = NULL;
    list->length--;
    return node;
}

void* l_list_pop_last(l_list* list) {
    l_node* node = list->end;
    if (node->prev) {
        list->end = node->prev;
    } else {
        list->end = NULL;
        list->start = NULL;
    }
    if (node->prev) node->prev->next = NULL;
    node->prev = NULL;
    node->next = NULL;
    list->length--;
    void* data = node->data;
    free(node);
    return data;
}