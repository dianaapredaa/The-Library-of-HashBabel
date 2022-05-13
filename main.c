// Copyright Preda Diana 314CA 2021 - 2022
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashmap.h"
#include "structures.h"
#include "book_commands.h"
#include "def_commands.h"
#include "user_commands.h"
#include "top.h"

int main(void)
{
	char action[AMAX];
	// books hastable
	hashtable_t *library = ht_create(HMAX, hash_function_string,
	compare_function_strings);
	// users hastable
	hashtable_t *users = ht_create(HMAX, hash_function_string,
	compare_function_strings);

	// read until command EXIT is introduced
	do {
		scanf("%s", action);

		if (strcmp(action, "ADD_BOOK") == 0) {
			add_book(&library);

		} else if (strcmp(action, "GET_BOOK") == 0) {
			get_book(library);

		} else if (strcmp(action, "RMV_BOOK") == 0) {
			rmv_book(library);

		} else if (strcmp(action, "ADD_DEF") == 0) {
			add_def(library);

		} else if (strcmp(action, "GET_DEF") == 0) {
            get_def(library);

		} else if (strcmp(action, "RMV_DEF") == 0) {
            rmv_def(library);

		} else if (strcmp(action, "ADD_USER") == 0) {
            add_user(&users);

		} else if (strcmp(action, "BORROW") == 0) {
            borrow(users, library);

		} else if (strcmp(action, "RETURN") == 0) {
            return_book(users, library);

		} else if (strcmp(action, "LOST") == 0) {
            lost(users, library);

		} else if (strcmp(action, "EXIT") == 0) {
			// before quiting display top books and users
            top_books(library);
			top_users(users);
			exit_library(&library);
			exit_users(&users);
		}
	} while (strcmp(action, "EXIT") != 0);

	return 0;
}
