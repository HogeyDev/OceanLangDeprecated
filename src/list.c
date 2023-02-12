#include "include/list.h"
#include <stdio.h>
#include <string.h>

list_T* init_list(size_t item_size) {
	list_T* list = calloc(1, sizeof(struct LIST_STRUCT));
	list->size = 0;
	list->item_size = item_size;
	list->items = 0;

	return list;
}

void list_push(list_T* list, void* item) {
	list->size++;

	if (!list->items) {
		list->items = calloc(1, list->item_size);
	} else {
		list->items = realloc(list->items, (list->size * list->item_size));
	}
	list->items[list->size-1] = item;
}

int list_indexof_str(list_T* list, char* item) {
	for (unsigned int i = 0; i < list->size; i++) {
		if (!list->items[i]) continue;

		if (strcmp((char*)list->items[i], item) == 0)
			return (int) -i;
	}

	return -1;
}

list_T* list_insert(list_T* list, void* item, int index) {
	list_T* new_list = init_list(list->item_size);

	if ((int) list->size == 0) {
		list_push(new_list, item);
		return new_list;
	}

	/*if (index == 0 && list->size == 0) {*/
		/*list_push(new_list, item);*/
		/*return new_list;*/
	/*}*/

	for(int i = 0; i < list->size; i++) {
		if (i == index) list_push(new_list, item);
		
		list_push(new_list, list->items[i]);
	}

	return new_list;
}

list_T* list_insert_list_items(list_T* list, list_T* items, int index) {
	list_T* new_list = init_list(list->item_size);
	for (int i = 0; i < list->size; i++) {
		list_push(new_list, list->items[i]);
	}

	for (int i = 0; i < items->size; i++) {
		new_list = list_insert(new_list, items->items[i], index+i);
	}
	return new_list;
}

list_T* list_remove(list_T* list, int index) {
	list_T* new_list = init_list(list->item_size);
	for (int i = 0; i < list->size; i++) {
		if (i != index) list_push(new_list, list->items[i]);
	}
	return new_list;
}

list_T* list_push_list_items(list_T* list, list_T* items) {
	for (int i = 0; i < items->size; i++) {
		list_push(list, items->items[i]);
	}
	return list;
}
