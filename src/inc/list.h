#pragma once

#include <stdlib.h>

typedef struct LIST_STRUCT {
	void** items;
	size_t size;
	size_t item_size;
} list_T;

list_T* init_list(size_t item_size);

void list_push(list_T* list, void* item);

int list_indexof_str(list_T* list, char* item);

list_T* list_insert(list_T* list, void* item, int index);

list_T* list_insert_list_items(list_T* list, list_T* items, int index);

list_T* list_remove(list_T* list, int index);

list_T* list_push_list_items(list_T* list, list_T* items);
