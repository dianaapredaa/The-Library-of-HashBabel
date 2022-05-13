// Copyright 2021 - 2022 Preda Diana 314CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "structures.h"
#include "hashmap.h"
#include "def_commands.h"
#include "utils.h"

//  add new definition
void add_def(hashtable_t *ht)
{
    char str[EMAX];

    fgets(str, sizeof(str), stdin);

    char* name = strtok(str, "\"");
    name = strtok(NULL, "\"");

    char *def_key = strtok(NULL, " ");

    char *def_value = strtok(NULL, "\n");

    // check if the book exists
    if (!ht_has_key(ht, name)) {
        printf("The book is not in the library.\n");
        return;
    }

    book_struct *book;
    book = (book_struct *)ht_get(ht, name);

    // add definition
    ht_put(book->def, def_key, strlen(def_key) + 1, def_value,
    strlen(def_value) + 1);

    // if load factor > 1, resize and rehash hashtable
    double load_factor = (double)ht->size / (double)ht->hmax;
    if (load_factor > 1) {
        ht_resize_string(&book->def);
    }
}

// display a definition
void get_def(hashtable_t *ht)
{
    char str[AMAX];

    fgets(str, sizeof(str), stdin);

    char *name = strtok(str, "\"");
    name = strtok(NULL, "\"");
    char *def_key = strtok(NULL, "\n");

    for (unsigned int i = 0; i <= strlen(def_key); i++) {
        def_key[i] = def_key[i + 1];
    }

    // check if the book exists
    if (!ht_has_key(ht, name)) {
        printf("The book is not in the library.\n");
        return;
    }

    book_struct *book;
    book = (book_struct *)ht_get(ht, name);

    hashtable_t *def = (hashtable_t *)book->def;

    // check if the definition exists
    if (!ht_has_key(def, def_key)) {
        printf("The definition is not in the book.\n");
        return;
    }

    char* definition = (char*)ht_get(def, def_key);
    puts(definition);
}

// remove an indicated definition
void rmv_def(hashtable_t *ht)
{
    char str[AMAX];

    fgets(str, sizeof(str), stdin);

    char* name = strtok(str, "\"");
    name = strtok(NULL, "\"");

    char* def_key = strtok(NULL, "\n");

    for (unsigned int i = 0; i <= strlen(def_key); i++) {
        def_key[i] = def_key[i + 1];
    }

    // check if the book exists
    if (!ht_has_key(ht, name)) {
        printf("The book is not in the library.\n");
        return;
    }

    book_struct *book;
    book = (book_struct *)ht_get(ht, name);

    hashtable_t *def = (hashtable_t *)book->def;

    //  check if the definition exists
    if (ht_has_key(def, def_key))
        // remove the definition
        ht_remove_entry(def, def_key);
    else
        printf("The definition is not in the book.\n");
}
