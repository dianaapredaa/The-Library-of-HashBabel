// Copyright 2021 - 2022 Preda Diana 314CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "hashmap.h"
#include "structures.h"
#include "utils.h"
#include "user_commands.h"

//  add new user
void add_user(hashtable_t **users_adr)
{
    hashtable_t* users = *users_adr;
    user_struct *user;
    user = malloc(sizeof(user_struct));
    DIE(NULL == user, "malloc failed");

    char name[CMAX];

    scanf("%s", name);

    // check if the user already exists
    if (ht_has_key(users, name)) {
        printf("User is already registered.\n");
        free(user);
        return;
    }

    // put name
    user->name = malloc(strlen(name) + 1);
    DIE(NULL == user->name, "malloc failed");
    memcpy(user->name, name, strlen(name) + 1);

    // value initialization
    user->score = 100;
    user->allowed = 0;
    user->borrow =  0;
    user->borrowed_book = NULL;
    user->promissed_days = 0;

    // add user into the hastable
    ht_put(users, user->name, strlen(user->name) + 1, user,
    sizeof(user_struct));

    *users_adr = users;

    double load_factor = (double)users->size / (double)users->hmax;
    if (load_factor > 1) {
        ht_resize_struct(users_adr, sizeof(user_struct));
    }

    free(user);
}

// user borrow book
void borrow(hashtable_t *users, hashtable_t *ht) {
    char str[AMAX];

    fgets(str, sizeof(str), stdin);

    char* user_name = strtok(str, " ");

    char* book_name = strtok(NULL, "\"");

    char *days = strtok(NULL, "\n");
    int days_number = atoi(days);

    // check if the user is registered
    if (!ht_has_key(users, user_name)) {
        printf("You are not registered yet.\n");
        return;
    }

    user_struct *user;
    user = (user_struct *)ht_get(users, user_name);

    // check if the user id banned
    if (user->allowed == 1) {
        printf("You are banned from this library.\n");
        return;
    }

    // check if the user have already borrowed a book
    if (user->borrow == 1) {
        printf("You have already borrowed a book.\n");
        return;
    }

    // check if the book is in the library
    if (!ht_has_key(ht, book_name)) {
        printf("The book is not in the library.\n");
        return;
    }

    book_struct *book;
    book = (book_struct *)ht_get(ht, book_name);

    if (book == NULL) {
        printf("The book is not in the library.\n");
        return;
    }

    // check if the book is already borrowed
    if (book->available == 1 || book->borowee != NULL) {
        printf("The book is borrowed.\n");
        return;
    }

    // update book status
    book->available = 1;
    book->borowee = user;

    // update user status
    user->borrow = 1;
    user->borrowed_book = book;
    user->promissed_days = days_number;
}

// user return book
void return_book(hashtable_t *users, hashtable_t *ht)
{
    char str[AMAX];

    fgets(str, sizeof(str), stdin);

    char* user_name = strtok(str, " ");

    char* book_name = strtok(NULL, "\"");

    char *days = strtok(NULL, " ");
    int days_since_borrow = atoi(days);

    char *book_rating = strtok(NULL, "\n");
    int rating = atoi(book_rating);

    // check if the user is registered
    if (!ht_has_key(users, user_name)) {
        printf("You are not registered yet.\n");
        return;
    }

    // check if the book is in the library
    if (!ht_has_key(ht, book_name)) {
        printf("The book is not in the library.\n");
        return;
    }

    user_struct *user;
    user = (user_struct *)ht_get(users, user_name);

    book_struct *book;
    book = (book_struct *)ht_get(ht, book_name);

    if (book == NULL) {
        printf("The book is not in the library.\n");
        return;
    }

    // check if the user is banned
    if (user->allowed == 1) {
        printf("You are banned from this library.\n");
        return;
    }

    // check if the user borrowed a book
    if (user->borrow == 0 || user->borrowed_book == NULL
        || book->available == 0 || book->borowee != user) {
        printf("You didn't borrow this book.\n");
        return;
    }

    // check if the user borrowed the indicated book
    if (strcmp(user->borrowed_book->name, book_name)) {
        printf("You didn't borrow this book.\n");
        return;
    }

    // calculate new score
    if (user->promissed_days < days_since_borrow) {
        user->score -= (days_since_borrow - user->promissed_days) * 2;
    }

    if (user->promissed_days > days_since_borrow) {
        user->score += (user->promissed_days - days_since_borrow);
    }

    // ban user if necessary
    if (user->score < 0) {
        user->allowed = 1;
        printf("The user %s has been banned from this library.\n", user_name);
    }

    // update user status
    user->borrow = 0;
    user->promissed_days = 0;
    user->borrowed_book = NULL;

    // update book status
    book->purchases++;
    book->available = 0;
    book->rating += rating;
    book->borowee = NULL;
}

// user lost borrowed book
void lost(hashtable_t *users, hashtable_t *ht) {
    char str[AMAX];

    fgets(str, sizeof(str), stdin);

    char* user_name = strtok(str, " ");
    user_name[strlen(user_name)] = '\0';

    char* book_name = strtok(NULL, "\"");

    // check if the user is registered
    if (!ht_has_key(users, user_name)) {
        printf("You are not registered yet.\n");
        return;
    }

    // check if the book is in the library
    if (!ht_has_key(ht, book_name)) {
        printf("The book is not in the library.\n");
        return;
    }

    user_struct *user;
    user = (user_struct *)ht_get(users, user_name);

    book_struct *book;
    book = (book_struct *)ht_get(ht, book_name);

    if (book == NULL) {
        printf("The book is not in the library.\n");
        return;
    }

    // check if the user is banned
    if (user->allowed == 1) {
        printf("You are banned from this library.\n");
        return;
    }

    // update user status
    user->borrow = 0;
    user->promissed_days = 0;
    user->borrowed_book = NULL;

    free(book->name);
    ht_free(book->def);

    // remove the lost book
    ht_remove_entry(ht, book_name);

    // update user score
    user->score -= 50;

    // ban user if necessary
    if (user->score < 0) {
        user->allowed = 1;
        printf("The user %s has been banned from this library.\n", user_name);
    }
}
