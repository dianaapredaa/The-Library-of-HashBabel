// Copyright 2021 - 2022 Preda Diana 314CA

#ifndef USER_COMMANDS_H_
#define USER_COMMANDS_H_

#include "hashmap.h"

void add_user(hashtable_t **users_adr);

void lost(hashtable_t *users, hashtable_t *ht);

void borrow(hashtable_t *users, hashtable_t *ht);

void return_book(hashtable_t *users, hashtable_t *ht);

void exit_users(hashtable_t **users_adr);

#endif  // USER_COMMANDS_H_
