/**
 * comp2129 - assignment 2
 * Cian Byrne
 * cbyr2401
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <ctype.h>  // toupper()

#include "snapshot.h"

entry* entry_head = NULL;
entry* entry_tail = NULL;

snapshot* snapshot_head = NULL;
snapshot* snapshot_tail = NULL;

char * argv[MAX_COMMAND];

//
// We recommend that you design your program to be
// modular where each function performs a small task
//
// e.g.
//
// command_bye
// command_help
// ...
// entry_add
// entry_delete
// ...
//



void (*ptrcommand[28])() = {
	command_bye, command_help, command_get,
	command_del, command_purge, command_set,
	command_push, command_append, command_pick,
	command_pluck, command_pop, command_drop,
	command_rollback, command_checkout, command_snapshot,
	command_min, command_max, command_sum, command_len,
	command_rev, command_uniq, command_sort,
	command_diff, command_inter, command_union,
	command_listKeys, command_listEntries, command_listSnapshots
};

// from solutions from week 5 tutorial
// void list_init(entry* head){
	// head->next = head;
	// head->prev = head;
// }

// void list_push(node* head, node* n) {
	// n->prev = head;
	// n->next = head->next;

	// n->next->prev = n;
	// n->prev->next = n;
// }
// // end week 5


int entry_empty(entry* head){
	return (head==NULL);
}

int snapshot_empty(snapshot* head){
	return (head==NULL);
}

// source: http://stackoverflow.com/questions/8287109/how-to-copy-one-integer-array-to-another
int* create_values(int const * arr){
	size_t length = sizeof(arr)/sizeof(int);
	int * ptr = malloc(length * sizeof(int));
	memcpy(ptr, arr, length * sizeof(int));
	return ptr;
}

int arrlen(const char* arr){
	return (sizeof(argv)/sizeof(argv[0]));
}

// function to clean argv global 
void clean_argv(void){
	for(int i = 0; i < arrlen(*argv); i++) argv[i] = 0;
}

entry* entry_create(char* argv){
	entry* n = (entry *) malloc(sizeof(entry));
	int* list = malloc(arrlen(argv)-2 * sizeof(int));

	for(int i = 0; i < (arrlen(argv)-2); i++){
		list[i] = (int)argv[i+2];
	}
	
	// set elements in new entry:
	n->key = argv[1];  //TODO: fix this!!!!!!!!!!!!!!!!!!!!!!
	n->values = list;
	
	// return pointer to new entry
	return n;
}

void entry_add(entry* head, entry* n){
	entry* current = head;
	if(current == NULL){
		// first element in list.
		entry_head = n;
	}
	// search the list until the end:
	while(current->next != NULL){
		current = head->next;
	}
	// create link
	current->next = n;
	n->prev = current;	
}

void entry_remove(entry* n){
	if(n->prev == NULL){
		// firt element in list
		entry_head = NULL;
		// remove n
		free(n->values);
		free(n);
	}else{
		// rebuild link, with n removed.
		n->next->prev = n->prev;
		n->prev->next = n->next;
		// remove n
		free(n->values);
		free(n);
	}
}

void entry_list(entry* head){
	if(head == NULL){
		// the list is empty
		printf("no entries");
	}else{
		entry* current = head;
		while(current != NULL){
			// list all the keys...
			printf(current->key);
		}
	}
}


int main(void) {

	char line[MAX_LINE];

	while (true) {
    	printf("> ");

    	if (fgets(line, MAX_LINE, stdin) == NULL) {
    	    //(*ptrc[0])();
    	    return 0; 		
    	}
		else{
			// TAG: moved argv[] to global scope
			char *word;
			const char delim[2] = " ";
			int nargs = 0;
			// clear the arguments array.
			clean_argv();
			
			// strip newline character:
			strtok(line, "\n");
			
			// get first argument
			argv[nargs] = strtok(line, delim);
			
			// sourced from: https://gist.github.com/efeciftci/9120921
			// get all other arguements:
			while((word = strtok(NULL, delim)) != NULL){
				nargs++;
				argv[nargs] = word;
			}
			
			if(nargs < 1){
				
			}
			// TODO: remove this and put in strcasecmp
			// convert all characters for command to uppercase for search...
			for(int c=0; c < strlen(argv[0]); c++) argv[0][c] = toupper(argv[0][c]);
			
			// check for "LIST"
			if(strcmp(argv[0], "LIST")==0){
				// TODO: remove this and put in strcasecmp
				// convert second word to upper case:
				for(int c=0; c < strlen(argv[1]); c++) argv[1][c] = toupper(argv[1][c]);
				// concat the two words together:
				//strcat(strcat(argv[0], delim), argv[1]);
				argv[0][strlen(argv[0])] = ' ';  // replaces '\0' with ' ', bit dangerous.
			}
			
			// look for a matching command and then run it.
			// TODO: put in strcasecmp
			for(int i = 0; i < 28; i++){
				if(strcmp(argv[0],COMMAND_LIST[i])==0) (*ptrcommand[i])();
			}	
			
		}
			
    	
	    //
	    // TODO
	    //
  	}

	return 0;
}


void command_bye () {
	// clean up all memory
	// printf
	// exit
    printf("bye\n");
	exit(0);
}

void command_help() {
    printf("%s\n", HELP);
}

void command_get(){
	
};
void command_del(){
	
};
void command_purge(){
	
};

void command_set(){
	
};
void command_push(){
	
};
void command_append(){
	
};

void command_pick(){
	
};
void command_pluck(){
	
};
void command_pop(){
	
};

void command_drop(){
	
};
void command_rollback(){
	
};
void command_checkout(){
	
};
void command_snapshot(){
	
};

void command_min(){
	
};
void command_max(){
	
};
void command_sum(){
	
};
void command_len(){
	
};

void command_rev(){
	
};
void command_uniq(){
	
};
void command_sort(){
	
};

void command_diff(){
	
};
void command_inter(){
	
};
void command_union(){
	
};

void command_listKeys(){
	
};
void command_listEntries(){
	
};
void command_listSnapshots(){
	
};