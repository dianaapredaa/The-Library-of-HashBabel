// Copyright 2021 - 2022 Preda Diana 314CA

#ifndef STRUCTURES_H_
#define STRUCTURES_H_

#include "hashmap.h"

#define HMAX 1
#define AMAX 100
#define EMAX 60
#define CMAX 20

typedef struct user_struct user_struct;

typedef struct book_struct {
    char *name;
    // number of purchases
    int purchases;
    double rating;
    // 0 for available, 1 for borrowed
    int available;
    user_struct *borowee;
    hashtable_t *def;
} book_struct;

typedef struct user_struct {
    char *name;
    // 0 for allowed, 1 for denied
    int allowed;
    int score;
    // 0 for no book borrowed, 1 for 1 book borrowed
    int borrow;
    int promissed_days;
    book_struct* borrowed_book;
} user_struct;

#endif  // STRUCTURES_H_
