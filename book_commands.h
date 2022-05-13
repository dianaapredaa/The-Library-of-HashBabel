// Copyright 2021 - 2022 Preda Diana 314CA

#ifndef BOOK_COMMANDS_H_
#define BOOK_COMMANDS_H_

#include "hashmap.h"

void add_book(hashtable_t **ht_adr);

void get_book(hashtable_t *ht);

void rmv_book(hashtable_t *ht);

void exit_library(hashtable_t **ht_adr);

double rating(int purchases, double rating);

#endif  // BOOK_COMMANDS_H_
