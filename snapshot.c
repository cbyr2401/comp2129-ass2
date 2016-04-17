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

char * argv[MAX_COMMAND];
int next_snapshot_id = 1;


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

/*
 *	Description: 	Removes all elements in global argv (sets to NULL)
 * 	Return:			none.
 */
void clean_argv(void){
	for(int i = 0; i < ARRLEN(argv); i++) argv[i] = NULL;
}

/*
 *	Description: 	Creates a new entry.  Uses values stored in global argv, 
 *					 populated by command line.
 *	Return:			Pointer to newly created entry.
 */
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

/*
 *	Description: 	Changes values in provided entry.
 *					 NOTE: This method is unsafe.  Only use with SET.
 * 	Return:			none.
 */
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

/*
 *	Description: 	Adds provided entry to back of linked list (entry_head).
 *					 NOTE: This method is unsafe.
 *	Return:			none.
 */
void entry_append(entry* head, entry* n){
	entry* current = head;
	if(current == NULL){
		// first element in list.
		entry_head = n;
		entry_tail = n;
		n->next = NULL;
		n->prev = NULL;
	}else{
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

/*
 *	Description: 	Adds provided entry to front of linked list (entry_head).
 *					 NOTE: This method is unsafe.
 *	Return:			none.
 */
void entry_push(entry* head, entry* n){
	entry* current = head;
	if(current == NULL){
		// first element in list.
		entry_head = n;
		entry_tail = n;
		n->next = NULL;
		n->prev = NULL;
	}else{
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

/*
 *	Description: 	Searches provided linked list for a entry given key.
 *	Return:			Pointer to located entry or NULL (not located).
 */
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

/*
 *	Description: 	Searches provided linked list for a snapshot.
 *	Return:			Pointer to located snapshot or NULL (not located).
 */
snapshot* snapshot_find(snapshot* head, int id){
	// search the list until the end:
	snapshot* current = head;
	if(head == NULL) return NULL;
	while(current->next != NULL && current->id != id ){
		current = current->next;
	}
	if(current->id == id) return current;
	else return NULL;
}

/*
 *	Description: 	Removes provided entry from linked list (entry_head).
 *					 NOTE: This method is unsafe.
 *	Return:			none.
 */
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

/*
 *	Description: 	Removes all entries from linked list (entry_head).
 *					 NOTE: This method is unsafe because it calls entry_remove()
 *	Return:			none.
 */
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

/*
 *	Description: 	free() entry from memory (SAFE).
 *	Return: 		value of next in struct or NULL (last element in list)
 */
entry* entry_free(entry* n){
	if(n == NULL) return NULL;
	// store next:
	entry* next = n->next;
	// free memory of values, then the struct
	free(n->values);
	free(n);
	return next;
}

/*
 *	Description: 	free() all entries in provied linked-list from memory (SAFE).
 *	Return: 		none.
 */
void entry_freeList(entry* n){
	entry* current = n;
	while(current != NULL){
		current = entry_free(current);
	}
}

/*
 *	Description: 	Creates a duplicate of the provided entry.
 *	Return: 		Pointer to new entry.
 */
entry* entry_copy(entry* master){
	entry* copy = (entry *) malloc(sizeof(entry));
	int length = master->length;
	
	// building list for copy entry:	
	int* list = (int *)malloc(length * sizeof(int));
	
	// copy the values list::
	for(int i = 0; i < length; i++) list[i] = master->values[i];


	// load up the entry with values:
	strcpy(copy->key, master->key);
	copy->values = list;
	copy->length = length;
	
	// this function is finished...
	return copy;
}

/*
 *	Description: 	Creates a duplicate of the provided linked-list.
 *	Return: 		Pointer to new linked-list.
 */
entry* entry_listCopy(entry* head){
	entry* current = head;
	entry* copy;
	entry* new_head = NULL;
	entry* new_tail = NULL;
	
	if(head==NULL){
		// no entries in list to snapshot
	}else{
		while(current != NULL){
			copy = entry_copy(current);
			if(new_head == NULL){
				// first element in list.
				new_head = copy;
				new_tail = copy;
				copy->next = NULL;
				copy->prev = NULL;
			}else{
				// create link
				new_tail->next = copy;
				copy->prev = new_tail;
				copy->next = NULL;
				// for rapid fire adding of elements:
				new_tail = copy;
			}
			current = current->next;
		}	
	}
	return new_head;

}

/*
 *	Description: 	Adds provided snapshot to linked-list (UNSAFE).
 *	Return: 		none.
 */
void snapshot_append(snapshot* head, snapshot* n){
	snapshot* current = head;
	if(current == NULL){
		// first element in list.
		snapshot_head = n;
		snapshot_tail = n;
		n->next = NULL;
		n->prev = NULL;
	}else{
		// for rapid fire adding of elements:
		current = snapshot_tail;
		
		// create link
		current->next = n;
		n->next = NULL;
		n->prev = current;
		// for rapid fire adding of elements:
		snapshot_tail = n;
	}	
}

/*
 *	Description: 	Removes provided snapshot from linked-list (UNSAFE).
 *	Return: 		none.
 */
void snapshot_remove(snapshot* n){
	if(n->prev == NULL && n->next == NULL){
		// only element in list
		snapshot_head = NULL;
		// free memory of values, then the struct
		entry_freeList(n->entries);
		free(n);
	}else if(n->prev == NULL && n->next != NULL){
		// we are dealing with the first element:
		snapshot_head = n->next;
		n->next->prev = NULL;
		// free memory of values, then the struct
		entry_freeList(n->entries);
		free(n);
	}else if(n->next == NULL && n->prev != NULL){
		// we are dealing with the last element:
		snapshot_tail = n->prev;
		n->prev->next = NULL;
		// free memory of values, then the struct
		entry_freeList(n->entries);
		free(n);
	}else{
		// rebuild link, with n removed.
		n->next->prev = n->prev;
		n->prev->next = n->next;
		// free memory of values, then the struct
		entry_freeList(n->entries);
		free(n);
	}
}

/*
 *	Description: 	Removes all snapshots from linked list (snapshot_head).
 *					 NOTE: This method is unsafe because it calls entry_remove()
 *	Return:			none.
 */
void snapshot_removeAll(snapshot* head){
	snapshot* current = head;
	snapshot* next = NULL;
	if(head == NULL){
		// nothing to clean up :)
	}else{
		// free all memory from each individual entry:
		while(current != NULL){
			next = current->next;
			snapshot_remove(current);
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
	entry_freeList(entry_head);
	// snapshot clean all... bit harder.
	snapshot_removeAll(snapshot_head);
	// say bye:
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
	
	
	// TODO
	
	
	printf("ok\n");
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
		if(index < 1 || index > n->length){
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
		if(index < 1 || index > n->length){
			printf("index out of range\n");
		}else{
			int value = n->values[index-1];
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
	int index = atoi(argv[1]);
	snapshot* n = snapshot_find(snapshot_head, index);
	if(n == NULL){
		printf("no such snapshot\n");
	}else{
		// remove snapshot from snapshot_head
		snapshot_remove(n);
		printf("ok\n");
	}
};

void command_rollback(){
	int index = atoi(argv[1]);
	snapshot* n = snapshot_find(snapshot_head, index);
	if(n == NULL){
		printf("no such snapshot\n");
	}else{
		// remove all entries to avoid memory leaks
		entry_freeList(entry_head);
		// insert new head from snapshot
		entry_head = entry_listCopy(n->entries);
		// remove all entries after current one:
		snapshot* current = n->next;
		snapshot* next = NULL;
		while(current != NULL){
			next = current->next;
			snapshot_remove(current);
			current = next;
		}
		// output
		printf("ok\n");
	}
};

void command_checkout(){
	int index = atoi(argv[1]);
	snapshot* n = snapshot_find(snapshot_head, index);
	if(n == NULL){
		printf("no such snapshot\n");
	}else{
		// remove all entries to avoid memory leaks
		entry_freeList(entry_head);
		// insert new head from snapshot
		entry_head = entry_listCopy(n->entries);
		// output
		printf("ok\n");
	}
};

void command_snapshot(){
	// create new snapshot
	snapshot* snap = (snapshot*)malloc(sizeof(snapshot));
	// assign new snapshot an id
	snap->id = next_snapshot_id;
	// assign new snapshot current state:
	snap->entries = entry_listCopy(entry_head);
	// add new snapshot to linked-list:
	snapshot_append(snapshot_head, snap);
	// print output
	printf("saved as snapshot %d\n", next_snapshot_id);
	// increment id (to maintain unique-ness)
	next_snapshot_id++;
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