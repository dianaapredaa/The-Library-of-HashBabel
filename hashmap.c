// Copyright 2021 - 2022 Preda Diana 314CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashmap.h"
#include "utils.h"
#include "structures.h"

#define MAX_STRING_SIZE	256

// create list
linked_list_t *
ll_create(unsigned int data_size)
{
    linked_list_t* ll;

    ll = malloc(sizeof(*ll));
    DIE(NULL == ll, "malloc failed");

    ll->head = NULL;
    ll->data_size = data_size;
    ll->size = 0;

    return ll;
}

// get an indicated node out of the list
ll_node_t*
 ll_get_nth_node(linked_list_t* list,  int n)
{
    ll_node_t *node;

    if (!list || !list->size)
        return NULL;

    node = list->head;
    n = n % list->size;

    for ( int i = 0; i < n; i++)
        node = node->next;

    return node;
}

// add a new node on an indicated position
void
ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data)
{
    ll_node_t *prev, *curr;
    ll_node_t* new_node;

    if (!list) {
        return;
    }

    if (n > list->size) {
        n = list->size;
    }

    curr = list->head;
    prev = NULL;
    while (n > 0) {
        prev = curr;
        curr = curr->next;
        --n;
    }

    new_node = malloc(sizeof(*new_node));
    DIE(NULL == new_node, "malloc failed");
    new_node->data = malloc(list->data_size);
    memcpy(new_node->data, new_data, list->data_size);

    new_node->next = curr;
    if (prev == NULL) {
        list->head = new_node;
    } else {
        prev->next = new_node;
    }

    list->size++;
}

// remove an indicated node
ll_node_t *
ll_remove_nth_node(linked_list_t* list, unsigned int n)
{
    ll_node_t *prev, *curr;

    if (!list || !list->head) {
        return NULL;
    }

    if (n > list->size - 1) {
        n = list->size - 1;
    }

    curr = list->head;
    prev = NULL;
    while (n > 0) {
        prev = curr;
        curr = curr->next;
        --n;
    }

    if (prev == NULL) {
        list->head = curr->next;
    } else {
        prev->next = curr->next;
    }

    list->size--;

    return curr;
}

// compare key function for int
int
compare_function_ints(void *a, void *b)
{
	int int_a = *((int *)a);
	int int_b = *((int *)b);

	if (int_a == int_b) {
		return 0;
	} else if (int_a < int_b) {
		return -1;
	} else {
		return 1;
	}
}

// compare key function for string
int
compare_function_strings(void *a, void *b)
{
	char *str_a = (char *)a;
	char *str_b = (char *)b;

	return strcmp(str_a, str_b);
}

// get list size
unsigned int
ll_get_size(linked_list_t* list)
{
     if (!list) {
        return -1;
    }

    return list->size;
}

// free list
void
ll_free(linked_list_t** pp_list)
{
    ll_node_t* currNode;

    if (!pp_list || !*pp_list) {
        return;
    }

    while (ll_get_size(*pp_list) > 0) {
        currNode = ll_remove_nth_node(*pp_list, 0);
        free(currNode->data);
        currNode->data = NULL;
        free(currNode);
        currNode = NULL;
    }

    free(*pp_list);
    *pp_list = NULL;
}

// hash function for string
unsigned int
hash_function_string(void *a)
{
	unsigned char *puchar_a = (unsigned char*) a;
	int hash = 5381;
	int c;

	while ((c = *puchar_a++))
		hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */

	return hash;
}

// hash function for int
unsigned int
hash_function_int(void *a)
{
	unsigned int uint_a = *((unsigned int *)a);

	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = (uint_a >> 16u) ^ uint_a;
	return uint_a;
}

// create hashtable function
hashtable_t *
ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
		int (*compare_function)(void*, void*))
{
    hashtable_t *ht = malloc(sizeof(hashtable_t));
    DIE(NULL == ht, "malloc failed");
    ht->buckets = malloc(sizeof(linked_list_t*) * hmax);
    DIE(NULL == ht->buckets, "malloc failed");

    for (unsigned int i = 0; i < hmax; i++) {
        ht->buckets[i] = ll_create(sizeof(info));
    }

    ht->hmax = hmax;
    ht->size = 0;
    ht->compare_function = compare_function;
    ht->hash_function = hash_function;

    return ht;
}

// this function returns1 if the key already exists
int
ht_has_key(hashtable_t *ht, void *key)
{
    if (ht == NULL) {
        return 0;
    }

    int hash_key = ht->hash_function(key);
    hash_key %= ht->hmax;
    linked_list_t *bucket_list = (linked_list_t*)ht->buckets[hash_key];
    if (ht->buckets[hash_key]->size == 0)
        return 0;
    ll_node_t *current_key_node = bucket_list->head;
    info *current_key_data;
    void *current_key;

    while (current_key_node) {
        current_key_data = current_key_node->data;
        current_key = current_key_data->key;
        if (ht->compare_function(key, current_key) == 0) {
            return 1;
        } else {
            current_key_node = current_key_node->next;
        }
    }

	return 0;
}

void *
ht_get(hashtable_t *ht, void *key)
{
    int hash_key = ht->hash_function(key);
    hash_key %= ht->hmax;
    linked_list_t *bucket_list = (linked_list_t*)ht->buckets[hash_key];
    if (ht->buckets[hash_key]->size == 0)
        return 0;
    ll_node_t *current_key_node = bucket_list->head;
    info *current_key_data;
    void *current_key;
    void *current_value;

    while (current_key_node) {
        current_key_data = current_key_node->data;
        current_key = current_key_data->key;
        current_value = current_key_data->value;
        if (ht->compare_function(key, current_key) == 0) {
            return current_value;
        } else {
            current_key_node = current_key_node->next;
        }
    }

	return NULL;
}

info *
ht_get_all(hashtable_t *ht, void *key)
{
    int hash_key = ht->hash_function(key);
    hash_key %= ht->hmax;
    linked_list_t *bucket_list = (linked_list_t*)ht->buckets[hash_key];
    if (ht->buckets[hash_key]->size == 0)
        return 0;
    ll_node_t *current_key_node = bucket_list->head;
    info *current_key_data;
    void *current_key;

    while (current_key_node) {
        current_key_data = current_key_node->data;
        current_key = current_key_data->key;
        if (ht->compare_function(key, current_key) == 0) {
            return current_key_data;
        } else {
            current_key_node = current_key_node->next;
        }
    }

	return NULL;
}

// add a new key - value pair
void
ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size)
{
    int hash_key = ht->hash_function(key);
    hash_key %= ht->hmax;
    linked_list_t *bucket_list = (linked_list_t*)ht->buckets[hash_key];

    // if the key exists already, modify only the value behind it
    if (ht_has_key(ht, key)) {
        info *info_ht = ht_get_all(ht, key);
        info_ht->value = realloc(info_ht->value, value_size);
        memcpy(info_ht->value, value, value_size);
        return;
    }

    info *info_ht = malloc(sizeof(info));
    DIE(NULL == info_ht, "malloc failed");
    info_ht->key = malloc(key_size);
    DIE(NULL == info_ht->key, "malloc failed");
    info_ht->value = malloc(value_size);
    DIE(NULL == info_ht->value, "malloc failed");

    memcpy(info_ht->key, key, key_size);
    memcpy(info_ht->value, value, value_size);

    ll_add_nth_node(bucket_list, bucket_list->size, info_ht);
    ht->size++;
    free(info_ht);
}

// remove an indicated entry
void
ht_remove_entry(hashtable_t *ht, void *key)
{
    int hash_key = ht->hash_function(key);
    hash_key %= ht->hmax;
    linked_list_t *bucket_list = ht->buckets[hash_key];
    ll_node_t *current_key_node = bucket_list->head;
    info *current_key_data = current_key_node->data;
    void *current_key = current_key_data->key;

    int i = 0;

    while (current_key_node) {
        if (ht->compare_function(key, current_key) == 0) {
            ll_node_t *trash = ll_remove_nth_node(bucket_list, i);
            info *trash_data = trash->data;

            free(trash_data->key);
            free(trash_data->value);
            free(trash->data);
            free(trash);
            break;
        } else {
            current_key_node = current_key_node->next;
            current_key_data = current_key_node->data;
            current_key = current_key_data->key;
        }

        i++;
    }
    ht->size--;
}

// free the alocated memory
void
ht_free(hashtable_t *ht)
{
    for (unsigned int i = 0; i < ht->hmax; i++) {
        linked_list_t *bucket_list = ht->buckets[i];

        int size = bucket_list->size;
        for (int j = 0; j < size; j++) {
            ll_node_t *trash = ll_remove_nth_node(bucket_list, j);
            info *trash_data = trash->data;
            free(trash_data->key);
            free(trash_data->value);
            free(trash->data);
            free(trash);
        }
        free(ht->buckets[i]);
    }

    free(ht->buckets);
    free(ht);
}

// resize hastable for strings
void ht_resize_string(hashtable_t **ht)
{
    hashtable_t *old_ht = *ht;
    int hmax = old_ht->hmax;

    hashtable_t *new_ht = ht_create(hmax * 2, hash_function_string,
									compare_function_strings);
	DIE(!new_ht, "hashtable resize failed");
	new_ht->hmax = 2 * hmax;

    for (int i = 0; i < hmax; ++i) {
        ll_node_t *it = old_ht->buckets[i]->head;

        while (it != NULL) {
            struct info *data = (struct info *)it->data;

			unsigned int key_size = 1 + strlen((char *)data->key);
			unsigned int value_size = 1 + strlen((char *)data->value);

			ht_put(new_ht, data->key, key_size, data->value, value_size);

            it = it->next;
        }
    }
	ht_free(old_ht);
	*ht = new_ht;
}

// resize hastable for structures
void ht_resize_struct(hashtable_t **ht, int value_size)
{
    hashtable_t *old_ht = *ht;
    int hmax = old_ht->hmax;

    hashtable_t *new_ht = ht_create(hmax * 2, hash_function_string,
									compare_function_strings);
	DIE(!new_ht, "hashtable resize failed");

    for (int i = 0; i < hmax; ++i) {
        ll_node_t *it = old_ht->buckets[i]->head;

        while (it != NULL) {
            info *data = (info*)it->data;

            if (data == NULL || data->value == NULL || data->key == NULL) {
                it = it->next;
                continue;
            }

			unsigned int key_size = strlen((char*)data->key) + 1;

			ht_put(new_ht, data->key, key_size, data->value, value_size);

            // remake the links between borrower and borrowee
            if (value_size == sizeof(book_struct)) {
                book_struct *old_book = data->value;
                book_struct *book_buff = ht_get(new_ht, old_book->name);
                if (book_buff->borowee && book_buff->available == 1) {
                    book_buff->borowee = old_book->borowee;
                    old_book->borowee->borrowed_book = book_buff;
                }
            } else if (value_size == sizeof(user_struct)) {
                user_struct* old_user = data->value;
                user_struct* user_buff = ht_get(new_ht, old_user->name);
                if (user_buff->borrowed_book != NULL &&
                    user_buff->borrow == 1) {
                    user_buff->borrowed_book = old_user->borrowed_book;
                    if (user_buff->borrowed_book->borowee != NULL)
                        user_buff->borrowed_book->borowee = user_buff;
                }
            }
            it = it->next;
        }
    }
	ht_free(*ht);
	*ht = new_ht;
}
