// Copyright 2021 - 2022 Preda Diana 314CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "structures.h"
#include "hashmap.h"

// free the memory allocated for the books and definitions hastables
void exit_library(hashtable_t **ht_adr)
{
	hashtable_t *ht = *ht_adr;

    for (unsigned int i = 0; i < ht->hmax; i++) {
        linked_list_t *bucket_list = ht->buckets[i];
        int size = bucket_list->size;
        for (int j = 0; j < size; j++) {
            ll_node_t *trash = ll_remove_nth_node(bucket_list, j);
            info *trash_data = trash->data;
            free(trash_data->key);
			book_struct* trash_book = trash_data->value;
			free(trash_book->name);
			ht_free(trash_book->def);
			free(trash_book);
            free(trash->data);
            free(trash);
        }
        free(ht->buckets[i]);
    }

    free(ht->buckets);
    free(ht);
}

// free the memory allocated for the users hastables
void exit_users(hashtable_t **users_adr)
{
	hashtable_t *users = *users_adr;

    for (unsigned int i = 0; i < users->hmax; i++) {
        linked_list_t *bucket_list = users->buckets[i];
        int size = bucket_list->size;
        for (int j = 0; j < size; j++) {
            ll_node_t *trash = ll_remove_nth_node(bucket_list, j);
            info *trash_data = trash->data;
            free(trash_data->key);
			user_struct* user = trash_data->value;
			free(user->name);
			free(user);
            free(trash->data);
            free(trash);
        }
        free(users->buckets[i]);
    }

    free(users->buckets);
    free(users);
}
