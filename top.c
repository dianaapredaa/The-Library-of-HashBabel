// Copyright 2021 - 2022 Preda Diana 314CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "structures.h"
#include "hashmap.h"
#include "top.h"
#include "utils.h"

// calculate book rating
double rating(int purchases, double rating)
{
    double final_rating;

    if (purchases == 0) {
        final_rating = 0;
    } else {
        final_rating = rating / purchases;
    }

    return final_rating;
}

// display top books
void top_books(hashtable_t *ht)
{
    printf("Books ranking:\n");

    // auxiliar vector for books
    book_struct **book_array = malloc(sizeof(book_struct *) * ht->size);
    DIE(NULL == book_array, "malloc failed");
    book_struct *aux;

    int k = -1;

    // populate the array of books
    for (unsigned int i = 0; i < ht->hmax; i++) {
        linked_list_t *bucket_list = ht->buckets[i];
        for (unsigned int j = 0; j < bucket_list->size; j++)
        {
            ll_node_t *list01 = ll_get_nth_node(bucket_list, j);
            info *data01 = list01->data;
			book_struct* book = data01->value;
            book_array[++k] = book;
        }
    }

    double book01;
    double book02;

    int size = ht->size;
    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            book01 = rating(book_array[i]->purchases, book_array[i]->rating);
            book02 = rating(book_array[j]->purchases, book_array[j]->rating);

            // sort by rating
            if (book01 < book02) {
                aux = book_array[j];
                book_array[j] = book_array[i];
                book_array[i] = aux;
            }
            if (book01 == book02) {
                // sort by the number of purchases
                if (book_array[i]->purchases < book_array[j]->purchases) {
                    aux = book_array[j];
                    book_array[j] = book_array[i];
                    book_array[i] = aux;
                }
            }
        }
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            book01 = rating(book_array[i]->purchases, book_array[i]->rating);
            book02 = rating(book_array[j]->purchases, book_array[j]->rating);

            if (book01 == book02 && book_array[i]->purchases ==
            book_array[j]->purchases) {
                // sort alphabetical
                if (strcmp(book_array[i]->name,
                book_array[j]->name) < 0) {
                    aux = book_array[j];
                    book_array[j] = book_array[i];
                    book_array[i] = aux;
                }
            }
        }
    }

    double book;

    // print top books
    for (int i = 0; i < size; i++) {
        book = rating(book_array[i]->purchases,
        book_array[i]->rating);
        printf("%d. Name:%s Rating:%.3f Purchases:%d\n", i + 1,
        book_array[i]->name, book, book_array[i]->purchases);
    }
    free(book_array);
}

// display top users
void top_users(hashtable_t *ht)
{
    printf("Users ranking:\n");

    // auxiliar vector for users
    user_struct **user_array = malloc(sizeof(user_struct *) * ht->size);
    DIE(NULL == user_array, "malloc failed");
    int k = -1;

    // populate the array of users
    for (unsigned int i = 0; i < ht->hmax; i++) {
        linked_list_t *bucket_list = ht->buckets[i];
        for (unsigned int j = 0; j < bucket_list->size; j++) {
            ll_node_t *list01 = ll_get_nth_node(bucket_list, j);
            info *data01 = list01->data;
			user_struct* user = data01->value;
            if (user->allowed == 0 && user->score > 0) {
                user_array[++k] = user;
            }
        }
    }

    user_struct *aux;

    int size = k;
    for (int i = 0; i <= size - 1; i++) {
        for (int j = i + 1; j <= size; j++) {
            // sort by score
            if (user_array[i]->score < user_array[j]->score) {
                aux = user_array[j];
                user_array[j] = user_array[i];
                user_array[i] = aux;
            }
        }
    }

    for (int i = 0; i <= size; i++) {
        for (int j = 0; j <= size; j++) {
            if (user_array[i]->score == user_array[j]->score) {
                // sort alphabetical
                if (strcmp(user_array[i]->name, user_array[j]->name) < 0) {
                    aux = user_array[j];
                    user_array[j] = user_array[i];
                    user_array[i] = aux;
                }
            }
        }
    }

    // print top users
    for (int i = 0; i <= size; i++) {
        printf("%d. Name:%s Points:%d\n", i + 1, user_array[i]->name,
        user_array[i]->score);
    }
    free(user_array);
}
