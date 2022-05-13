// Copyright 2021 - 2022 Preda Diana 314CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "structures.h"
#include "hashmap.h"
#include "book_commands.h"
#include "utils.h"
#include "top.h"

//  add new book in the library
void add_book(hashtable_t **ht_adr)
{
    hashtable_t* ht = *ht_adr;
    book_struct *book;

    char str[AMAX];
    fgets(str, sizeof(str), stdin);

    char* name = strtok(str, "\"");
    name = strtok(NULL, "\"");

    char* value = strtok(NULL, "\n");
    int val = atoi(value);

    char def_key[CMAX];
    char def_value[CMAX];

    info* book_info;

    // check if the book already exists
    if (ht_has_key(ht, name)) {
        book_info = ht_get_all(ht, name);
        book = book_info->value;
        ht_free(book->def);
    } else {
        book = malloc(sizeof(book_struct));
        DIE(NULL == book, "malloc failed");
        book->name = malloc(strlen(name) + 1);
        DIE(NULL == book->name, "malloc failed");
        memcpy(book->name, name, strlen(name) + 1);
    }

    // create definions hastable
    book->def = ht_create(HMAX, hash_function_string, compare_function_strings);

    // read definitions
    for (int i = 0; i < val; i++) {
        scanf("%s%s", def_key, def_value);
        ht_put(book->def, def_key, strlen(def_key) + 1, def_value,
        strlen(def_value) + 1);
    }

    // value initialization
    book->purchases = 0;
    book->rating = 0.000;
    book->available = 0;
    book->borowee = NULL;

    // check if the book already exists
    if (ht_has_key(ht, name)) {
        book_info->value = book;
    } else {
        ht_put(ht, book->name, strlen(book->name) + 1, book,
        sizeof(book_struct));
        free(book);
    }

    *ht_adr = ht;

    double load_factor = (double)ht->size / (double)ht->hmax;
    if (load_factor > 1) {
        ht_resize_struct(ht_adr, sizeof(book_struct));
    }
}

// display book characteristics
void get_book(hashtable_t *ht)
{
    char str[AMAX];

    fgets(str, sizeof(str), stdin);

    char* name = strtok(str, "\"");
    name = strtok(NULL, "\"");

    if (!ht_has_key(ht, name)) {
        printf("The book is not in the library.\n");
        return;
    }

    book_struct *book;
    book = (book_struct *)ht_get(ht, name);

    // calculate book rating
    double book_rating = rating(book->purchases, book->rating);

    // display book characteristics
    printf("Name:%s Rating:%.3f Purchases:%d\n", book->name,
    book_rating, book->purchases);
}

void rmv_book(hashtable_t *ht)
{
    char str[AMAX];

    fgets(str, sizeof(str), stdin);

    char* name = strtok(str, "\"");
    name = strtok(NULL, "\"");

    // check if the book is in the library
    if (!ht_has_key(ht, name)) {
        printf("The book is not in the library.\n");
        return;
    }

    book_struct *book;
    book = (book_struct *)ht_get(ht, name);

    if (book->available == 1 && book->borowee != NULL
        && book->borowee->allowed != 0) {
        book->borowee->borrowed_book = NULL;
        book->borowee->borrow = 0;
        book->borowee->promissed_days = 0;
    }

    // remove book
    free(book->name);
    ht_free(book->def);

    ht_remove_entry(ht, name);
}
