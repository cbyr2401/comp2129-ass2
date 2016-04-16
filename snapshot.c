/**
 * comp2129 - assignment 2
 * Cian Byrne
 * cbyr2401
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <strings.h>
#include <stdbool.h>
#include <ctype.h>  // toupper()
#include <stddef.h>

#include "snapshot.h"

#define ARRLEN(x)  (sizeof(x) / sizeof((x)[0]))


entry* entry_head = NULL;
entry* entry_tail = NULL;

snapshot* snapshot_head = NULL;
snapshot* snapshot_tail = NULL;

char * argv[MAX_COMMAND]; // = malloc(MAX_COMMAND * sizeof(char));

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

// function to clean argv global 
void clean_argv(void){
	for(int i = 0; i < ARRLEN(argv); i++) argv[i] = NULL;
}

entry* entry_create(){
	entry* n = (entry *) malloc(sizeof(entry));
	int length = 0;
	
	// building list for entry:	
	int* list = (int *)malloc(1 * sizeof(int));
	
	if(argv[2] == NULL){
		// empty list
	}else if(argv[3] == NULL){
		// one item in list
		list[0] = atoi(argv[2]);
		length = 1;
	}else{
		// multiple items:
		length = 0;

		for(; argv[length+2] != NULL; length++){
			list = realloc(list, (length+1)*sizeof(int));
			list[length] = atoi(argv[length+2]);
			//sscanf(argv[length+2], "%d", &list[length]);
		}

	}
	
	// check key length:
	if(strlen(argv[1])>MAX_KEY){
		// some defined error action would normally occur here,
		//  but that is outside scope of assessment.
	}

	// load up the entry with values:
	strcpy(n->key, argv[1]);
	n->values = list;
	n->length = length;
	
	// this function is finished...
	return n;
}

void entry_update(entry* n){
	int length = 0;
	int* list = (int *)malloc(length * sizeof(int));
	free(n->values);
	
	for(; argv[length+2] != NULL; length++){
			list = realloc(list, length+1);
			sscanf(argv[length+2], "%d", &list[length]);
	}
	// update length:
	n->length = length;
	n->values = list;
}

void entry_append(entry* head, entry* n){
	entry* current = head;
	if(current == NULL){
		// first element in list.
		entry_head = n;
		entry_tail = n;
		n->next = NULL;
		n->prev = NULL;
	}else{
		// search the list until the end:
		//while(current->next != NULL){
			//current = current->next;
		//}
		// for rapid fire adding of elements:
		current = entry_tail;
		
		// create link
		current->next = n;
		n->prev = current;
		n->next = NULL;
		// for rapid fire adding of elements:
		entry_tail = n;
	}	
}

void entry_push(entry* head, entry* n){
	entry* current = head;
	if(current == NULL){
		// first element in list.
		entry_head = n;
		entry_tail = n;
		n->next = NULL;
		n->prev = NULL;
	}else{
		// search the list until the end:
		//while(current->next != NULL){
			//current = current->next;
		//}
		// for rapid fire adding of elements:
		current = entry_head;
		
		// create link
		current->prev = n;
		n->prev = NULL;
		n->next = current;
		// for rapid fire adding of elements:
		entry_head = n;
	}	
}

// searches given linked list for a key and then 
//  returns the address or NULL.
entry* entry_find(entry* head, char* key){
	// search the list until the end:
	entry* current = head;
	if(head == NULL) return NULL;
	while(current->next != NULL && strcmp(current->key, key) != 0 ){
		current = current->next;
	}
	if(strcmp(current->key, key) == 0) return current;
	else return NULL;
}

void entry_remove(entry* n){
	if(n->prev == NULL && n->next == NULL){
		// only element in list
		entry_head = NULL;
		// free memory of values, then the struct
		free(n->values);
		free(n);
	}else if(n->prev == NULL && n->next != NULL){
		// we are dealing with the first element:
		entry_head = n->next;
		n->next->prev = NULL;
		// free memory of values, then the struct
		free(n->values);
		free(n);
	}else if(n->next == NULL && n->prev != NULL){
		// we are dealing with the last element:
		entry_tail = n->prev;
		n->prev->next = NULL;
		// free memory of values, then the struct
		free(n->values);
		free(n);
	}else{
		// rebuild link, with n removed.
		n->next->prev = n->prev;
		n->prev->next = n->next;
		// free memory of values, then the struct
		free(n->values);
		free(n);
	}
}

void entry_removeAll(entry* head){
	entry* current = head;
	entry* next = NULL;
	if(head == NULL){
		// nothing to clean up :)
	}else{
		// free all memory from each individual entry:
		while(current != NULL){
			next = current->next;
			entry_remove(current);
			current = next;
		}
	}
}

// source: http://www.tutorialspoint.com/c_standard_library/c_function_qsort.htm
int sortcmp(const void * a, const void * b){
	return ( *(int*)a - *(int*)b );
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
				sprintf(argv[0], "%s %s", argv[0], argv[1]);
			}
			
			// look for a matching command and then run it.
			// TODO: put in strcasecmp
			
			for(int i = 0; i < 28; i++){
				if(strcmp(argv[0],COMMAND_LIST[i])==0) (*ptrcommand[i])();
			}
			printf("\n");
			
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
	entry_removeAll(entry_head);
	// snapshot clean all... bit harder.
    printf("bye\n");
	exit(0);
}

void command_help() {
    printf("%s", HELP);
}

void command_get(){
	entry* n = entry_find(entry_head, argv[1]);
	if(n == NULL){
		printf("no such key\n");
	}else{
		printf("[");
		for(int i = 0; i < n->length; i++){
			if(i==n->length-1){
				printf("%d", n->values[i]);
			} else{
				printf("%d ", n->values[i]);
			}
		}
		printf("]\n");
	}
};

void command_del(){
	entry* n = entry_find(entry_head, argv[1]);
	if(n == NULL){
		printf("no such key\n");
	}else{
		entry_remove(n);
		printf("ok\n");
	}
};

void command_purge(){
	
};

void command_set(){
	// check entry exists
	entry* n = entry_find(entry_head, argv[1]);
	if(entry_head == NULL || n == NULL){
		// create new entry and append to list
		n = entry_create();
		entry_push(entry_head, n);
	}else{
		// set values in existing entry
		entry_update(n);
	}	
	// operation completed.
	printf("ok\n");
};

void command_push(){
	entry* n = entry_find(entry_head, argv[1]);
	if(n == NULL){
		printf("no such key\n");
	}else{
		int length = n->length;
		for(int j = 2; argv[j] != NULL; j++){
				n->values = realloc(n->values, (length+1)*sizeof(int));
				// shift all values down by one:
				for(int i = length; i > 0; i--) n->values[i] = n->values[i-1];
				n->values[0] = atoi(argv[j]);
				length++;
		}
		n->length = length;
		printf("ok\n");
	}
};

void command_append(){
	entry* n = entry_find(entry_head, argv[1]);
	if(n == NULL){
		printf("no such key\n");
	}else{
		int length = n->length;
		for(int j = 2; argv[j] != NULL; j++){
				n->values = realloc(n->values, (length+1)*sizeof(int));
				n->values[length] = atoi(argv[j]);
				length++;
		}
		n->length = length;
		printf("ok\n");
	}
};

void command_pick(){
	entry* n = entry_find(entry_head, argv[1]);
	int index = atoi(argv[2]);
	if(n == NULL){
		printf("no such key\n");
	}else{
		if(index < 1 && index > n->length){
			printf("index out of range\n");
		}else{
			printf("%d\n", n->values[index-1]);
		}	
	}
};

void command_pluck(){
	entry* n = entry_find(entry_head, argv[1]);
	int index = atoi(argv[2]);
	if(n == NULL){
		printf("no such key\n");
	}else{
		if(index < 1 && index > n->length){
			printf("index out of range\n");
		}else{
			int value = n->values[index];
			// shift all values down by one slot.
			for(int i = index; i < n->length-1; i++) n->values[i] = n->values[i+1];
			n->values = realloc(n->values, (n->length-1)*sizeof(int));
			n->length = n->length-1;
			printf("%d\n", value);
		}	
	}
};

void command_pop(){
	entry* n = entry_find(entry_head, argv[1]);
	if(n == NULL){
		printf("no such key\n");
	}else{
		if(n->length == 0){
			printf("nil\n");
		}else{
			int value = n->values[0];
			// shift all values down by one slot.
			for(int i = 0; i < n->length-1; i++) n->values[i] = n->values[i+1];
			n->values = realloc(n->values, (n->length-1)*sizeof(int));
			n->length = n->length-1;
			printf("%d\n", value);
		}
	}
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
	entry* n = entry_find(entry_head, argv[1]);
	int min_value;
	if(n == NULL){
		printf("no such key\n");
	}else{
		if(n->length == 0){
			printf("nil\n");
		}else{
			min_value = n->values[0];
			for(int i = 1; i < n->length; i++){
				if(n->values[i] < min_value) min_value = n->values[i];
			}
			printf("%d\n", min_value);
		}
	}
};
void command_max(){
	entry* n = entry_find(entry_head, argv[1]);
	int max_value;
	if(n == NULL){
		printf("no such key\n");
	}else{
		if(n->length == 0){
			printf("nil\n");
		}else{
			max_value = n->values[0];
			for(int i = 1; i < n->length; i++){
				if(n->values[i] > max_value) max_value = n->values[i];
			}
			printf("%d\n", max_value);
		}
	}
};
void command_sum(){
	entry* n = entry_find(entry_head, argv[1]);
	
	if(n == NULL){
		printf("no such key\n");
	}else{
		if(n->length == 0){
			printf("nil\n");
		}else{
			int sum = 0;
			for(int i = 0; i < n->length; i++){
				sum += n->values[i];
			}
			printf("%d\n", sum);
		}
	}
};
void command_len(){
	entry* n = entry_find(entry_head, argv[1]);
	
	if(n == NULL){
		printf("no such key\n");
	}else{
		printf("%zu\n", n->length);
	}
	
};

void command_rev(){
	
};
void command_uniq(){
	
};
void command_sort(){
	entry* n = entry_find(entry_head, argv[1]);
	
	if(n == NULL){
		printf("no such key\n");
	}else{
		qsort(n->values, n->length, sizeof(int), sortcmp);
		printf("ok\n");
	}
};

void command_diff(){
	
};
void command_inter(){
	
};
void command_union(){
	
};

void command_listKeys(){
	if(entry_head == NULL){
		// the list is empty
		printf("no keys\n");
	}else{
		entry* current = entry_head;
		while(current != NULL){
			// list all the keys...
			printf("%s\n", current->key);
			current = current->next;
		}
	}
};
void command_listEntries(){
	if(entry_head==NULL){
		// no entries:
		printf("no entries\n");
	}else{
		entry* current = entry_head;
		while(current != NULL){
			printf("%s [", current->key);
			for(int i = 0; i < current->length; i++){
				if(i==(current->length)-1){
					printf("%d", current->values[i]);
				}else{
					printf("%d ", current->values[i]);
				}
			}
			printf("]\n");
			current = current->next;
		}
	}
};
void command_listSnapshots(){
	if(snapshot_head==NULL){
		// no snapshots:
		printf("no snapshots\n");
	}else{
		snapshot* current = snapshot_head;
		while(current != NULL){
			printf("%d\n", current->id);
			current = current->next;
		}
	}
};