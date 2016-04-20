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
#define NUM_COMMANDS 28
#define COMMAND_ARG_NUM 0
#define COMMAND_KEY_NUM 1
#define COMMAND_ID_NUM 1
#define COMMAND_VALUES_INITAL 2

#define DEBUG 1

entry* entry_head = NULL;
entry* entry_tail = NULL;

snapshot* snapshot_head = NULL;
snapshot* snapshot_tail = NULL;

char * argv[MAX_COMMAND];
int next_snapshot_id = 1;

// Declare function foot prints (TODO: move to header file after refractoring)
void clean_argv();
int sortcmp(const void * a, const void * b);
int uniq(int* values, int length);
array* intersection(array* first, array* second);
array* difference(array* first, array* second);
	
entry* entry_create();
void entry_update(entry* node);
void entry_append(entry* node);
void entry_push(entry* node);
entry* entry_find(entry* head, char * key);
void entry_remove(entry* node);
void entry_removeAll(entry* head);
entry* entry_free(entry* node);
void entry_freeList(entry* head);
entry* entry_copy(entry* master);
entry* entry_listCopy(entry* head);

void snapshot_append(snapshot* node);
void snapshot_push(snapshot* node);
void snapshot_remove(snapshot* node);
void snapshot_removeAll(snapshot* head);
snapshot* snapshot_find(snapshot* head, int id);



void (*ptrcommand[NUM_COMMANDS])() = {
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

int main(void) {

	char line[MAX_LINE];

	while (true) {
		if(DEBUG==0) printf("> ");
    	

    	if (fgets(line, MAX_LINE, stdin) == NULL) {
    	    //(*ptrc[0])();
    	    return 0; 		
    	}
		else{
			char *word;
			const char delim[2] = " ";
			int nargs = 0;
			// clear the arguments array.
			clean_argv();
			
			// strip newline character:
			strtok(line, "\n");
			
			// get first argument
			argv[nargs] = strtok(line, delim);
			
			// get all other arguements:
			while((word = strtok(NULL, delim)) != NULL){
				nargs++;
				argv[nargs] = word;
			}
			
			if(nargs < 1){
				
			}
			// convert all characters for command to uppercase for search...
			for(int c=0; c < strlen(argv[COMMAND_ARG_NUM]); c++) argv[COMMAND_ARG_NUM][c] = toupper(argv[0][c]);
			
			// check for "LIST"
			if(strcmp(argv[COMMAND_ARG_NUM], "LIST")==0){
				// convert second word to upper case:
				for(int c=0; c < strlen(argv[1]); c++) argv[1][c] = toupper(argv[1][c]);
				// concat the two words together:
				sprintf(argv[COMMAND_ARG_NUM], "%s %s", argv[COMMAND_ARG_NUM], argv[1]);
			}
			
			// look for a matching command and then run it.			
			for(int i = 0; i < NUM_COMMANDS; i++){
				if(strcmp(argv[COMMAND_ARG_NUM],COMMAND_LIST[i])==0) (*ptrcommand[i])();
			}
			
			if(DEBUG==0) printf("\n");
		}
  	}

	return 0;
}

/*
 *	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *   FUNCTIONS CALLED DIRECTLY BY COMMAND LINE
 *	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *	See HELP for what each function performs.
 */
void command_bye () {
	// destroy database
	entry_freeList(entry_head);
	// destroy snapshots
	snapshot_removeAll(snapshot_head);
	// say bye bye and exit
    printf("bye\n");
	exit(0);
}

void command_help() {
    printf("%s", HELP);
}

void command_get(){
	// search for entry with given <key>
	entry* n = entry_find(entry_head, argv[COMMAND_KEY_NUM]);
	
	if(n == NULL){
		printf("no such key\n");
	}else{
		// display list in accepted format:
		// > GET a
		// a [1 2 3 4]
		printf("[");
		for(int i = 0; i < n->length; i++){
			if(i == (n->length-1) ){
				printf("%d", n->values[i]);
			} else{
				printf("%d ", n->values[i]);
			}
		}
		printf("]\n");
	}
};

void command_del(){
	// search for entry with given <key>
	entry* n = entry_find(entry_head, argv[COMMAND_KEY_NUM]);
	
	if(n == NULL){
		printf("no such key\n");
	}else{
		// destroy entry:
		entry_remove(n);
		
		printf("ok\n");
	}
};

void command_purge(){
	// search for entry with given <key>
	entry* n = entry_find(entry_head, argv[COMMAND_KEY_NUM]);

	// remove from current database
	if(n != NULL) entry_remove(n);
	
	// search all snapshots for entry:
	if(snapshot_head == NULL){
		// fail silently...
	}else{
		// set first snapshot to HEAD.
		snapshot* current_snapshot = snapshot_head;
		// used for rebuilding link with snapshot->entries:
		entry* first_entry = current_snapshot->entries;
		
		// loop through all snapshots...
		while(current_snapshot != NULL){
			n = entry_find(current_snapshot->entries, argv[COMMAND_KEY_NUM]);
			first_entry = current_snapshot->entries;
			
			// if element is first in list, re-add link:
			if(strcmp(n->key, first_entry->key)==0) current_snapshot->entries = n->next;
			
			// remove element (if exists)
			if(n != NULL) entry_free(n);
			
			// move to next snapshot...
			current_snapshot = current_snapshot->next;
		}
	}
	// always prints ok:
	printf("ok\n");
};

void command_set(){
	// search for entry with given <key>
	entry* n = entry_find(entry_head, argv[COMMAND_KEY_NUM]);
	
	if(entry_head == NULL || n == NULL){
		// create new entry and append to list
		n = entry_create();
		entry_push(n);
	}else{
		// set values in existing entry
		entry_update(n);
	}	
	// operation complete
	printf("ok\n");
};

void command_push(){
	// search for entry with given <key>
	entry* n = entry_find(entry_head, argv[COMMAND_KEY_NUM]);
	
	if(n == NULL){
		printf("no such key\n");
	}else{
		/* Each time there is a new value to PUSH:
		 * 1) resize array
		 * 2) shift all elements right by 1
		 * 3) add new item to front of array
		 */
		int length = n->length;
		for(int j = COMMAND_VALUES_INITAL; argv[j] != NULL; j++){
			// resize array
			n->values = realloc(n->values, (length+1)*sizeof(int));
			
			// shift all values right by one:
			for(int i = length; i > 0; i--) n->values[i] = n->values[i-1];
			
			// add new element:
			n->values[0] = atoi(argv[j]);
			
			length++;
		}
		// update length of array in entry
		n->length = length;
		
		printf("ok\n");
	}
};

void command_append(){
	// search for entry with given <key>
	entry* n = entry_find(entry_head, argv[COMMAND_KEY_NUM]);
	
	if(n == NULL){
		printf("no such key\n");
	}else{
		/* Each time there is a new value to APPEND:
		 * 1) resize array
		 * 2) add new item to end of array
		 */
		int length = n->length;
		for(int j = COMMAND_VALUES_INITAL; argv[j] != NULL; j++){
			// resize array
			n->values = realloc(n->values, (length+1)*sizeof(int));
			
			// add new element
			n->values[length] = atoi(argv[j]);
			length++;
		}
		// update length of array in entry
		n->length = length;
		
		printf("ok\n");
	}
};

void command_pick(){
	// search for entry with given <key>
	entry* n = entry_find(entry_head, argv[COMMAND_KEY_NUM]);
	
	// get index from command line
	int index = atoi(argv[2]);
	
	if(n == NULL){
		printf("no such key\n");
	}else{
		// check if index in range
		if(index < 1 || index > n->length){
			printf("index out of range\n");
		}else{
			// output value at index
			printf("%d\n", n->values[index-1]);
		}	
	}
};

void command_pluck(){
	// search for entry with given <key>
	entry* n = entry_find(entry_head, argv[COMMAND_KEY_NUM]);
	
	// get index from command line
	int index = atoi(argv[2]);
	
	if(n == NULL){
		printf("no such key\n");
	}else{
		// check if index in range
		if(index < 1 || index > n->length){
			printf("index out of range\n");
		}else{
			/* To PLUCK element from entry...
			 * 1) print value at index
			 * 2) shift all elements left by 1 (starting at plucked index)
			 * 3) resize array and update length
			 */
			 // output value
			printf("%d\n", n->values[index-1]);
			
			// shift all values left by one slot.
			for(int i = index; i < n->length-1; i++) n->values[i] = n->values[i+1];
			
			// resize array
			n->values = realloc(n->values, (n->length-1)*sizeof(int));
			
			// update length
			n->length = n->length-1;
		}	
	}
};

void command_pop(){
	// search for entry with given <key>
	entry* n = entry_find(entry_head, argv[COMMAND_KEY_NUM]);
	
	if(n == NULL){
		printf("no such key\n");
	}else{
		if(n->length == 0){
			printf("nil\n");
		}else{
			/* To POP element from entry...
			 * 1) print value at index
			 * 2) shift all elements left by 1
			 * 3) resize array and update length
			 */
			 // output value
			printf("%d\n", n->values[0]);
			
			// shift all values left by one slot.
			for(int i = 0; i < n->length-1; i++) n->values[i] = n->values[i+1];
			
			// resize array
			n->values = realloc(n->values, (n->length-1)*sizeof(int));
			
			// update length
			n->length = n->length-1;
		}
	}
};

void command_drop(){
	// search for snapshot with given <id>
	snapshot* n = snapshot_find(snapshot_head, atoi(argv[COMMAND_ID_NUM]));
	
	if(n == NULL){
		printf("no such snapshot\n");
	}else{
		// remove snapshot from snapshot_head
		snapshot_remove(n);
		printf("ok\n");
	}
};

void command_rollback(){
	// search for snapshot with given <id>
	snapshot* n = snapshot_find(snapshot_head, atoi(argv[COMMAND_ID_NUM]));
	
	if(n == NULL){
		printf("no such snapshot\n");
	}else{
		// remove all entries to avoid memory leaks
		entry_freeList(entry_head);
		
		// insert new entries list from snapshot
		entry_head = entry_listCopy(n->entries);
		
		// remove all snapshots after current <id>:
		snapshot* current = n->prev;
		snapshot* previous = NULL;
		
		while(current != NULL){
			previous = current->prev;
			snapshot_remove(current);
			current = previous;
		}
		
		snapshot_head = n;
		
		// output
		printf("ok\n");
	}
};

void command_checkout(){
	// search for snapshot with given <id>
	snapshot* n = snapshot_find(snapshot_head, atoi(argv[COMMAND_ID_NUM]));
	
	if(n == NULL){
		printf("no such snapshot\n");
	}else{
		// remove all entries to avoid memory leaks
		entry_freeList(entry_head);
		
		// insert new entries list from snapshot
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
	snapshot_push(snap);
	
	// output snapshot <id>
	printf("saved as snapshot %d\n", next_snapshot_id);
	
	// increment <id> (to maintain unique-ness)
	next_snapshot_id++;
};

void command_min(){
	// search for entry with given <key>
	entry* n = entry_find(entry_head, argv[COMMAND_KEY_NUM]);
	
	if(n == NULL){
		printf("no such key\n");
	}else{
		// no elements in the list, print nil
		if(n->length == 0){
			printf("nil\n");
		}else{
			// set minimum value to first element
			int min_value = n->values[0];
			
			// find minimum value in list (1 or more elements)
			for(int i = 1; i < n->length; i++){
				// compare values
				if(n->values[i] < min_value) min_value = n->values[i];
			}
			
			// output min value
			printf("%d\n", min_value);
		}
	}
};

void command_max(){
	// search for entry with given <key>
	entry* n = entry_find(entry_head, argv[COMMAND_KEY_NUM]);
	
	if(n == NULL){
		printf("no such key\n");
	}else{
		// no elements in the list, print nil
		if(n->length == 0){
			printf("nil\n");
		}else{
			// set maximum value to first element
			int max_value = n->values[0];
			
			// find maximum value in list (1 or more elements)
			for(int i = 1; i < n->length; i++){
				// compare values
				if(n->values[i] > max_value) max_value = n->values[i];
			}
			
			// output max value
			printf("%d\n", max_value);
		}
	}
};

void command_sum(){
	// search for entry with given <key>
	entry* n = entry_find(entry_head, argv[COMMAND_KEY_NUM]);
	
	if(n == NULL){
		printf("no such key\n");
	}else{
		// no elements in the list, print nil
		if(n->length == 0){
			printf("nil\n");
		}else{
			int sum = 0;
			
			// add up all elements in the list
			for(int i = 0; i < n->length; i++){
				sum += n->values[i];
			}
			
			// output sum
			printf("%d\n", sum);
		}
	}
};

void command_len(){
	// search for entry with given <key>
	entry* n = entry_find(entry_head, argv[COMMAND_KEY_NUM]);
	
	if(n == NULL){
		printf("no such key\n");
	}else{
		// output length (stored in struct)
		printf("%zu\n", n->length);
	}
};

void command_rev(){
	// search for entry with given <key>
	entry* n = entry_find(entry_head, argv[COMMAND_KEY_NUM]);
	
	if(n == NULL){
		printf("no such key\n");
	}else{
		int temp;
		// swap entries... front -> back, back->front
		for(int i = 0; i < (n->length/2); i++){
			temp = n->values[i];
			n->values[i] = n->values[n->length-i-1];
			n->values[n->length-i-1] = temp;
		}
		// output
		printf("ok\n");
	}
};

void command_uniq(){
	// search for entry with given <key>
	entry* n = entry_find(entry_head, argv[COMMAND_KEY_NUM]);
	
	if(n == NULL){
		printf("no such key\n");
	}else{
		
		n->length = uniq(n->values, n->length);
		n->values = realloc(n->values, n->length*sizeof(int));
				
		// output
		printf("ok\n");
	}
};

void command_sort(){
	// search for entry with given <key>
	entry* n = entry_find(entry_head, argv[COMMAND_KEY_NUM]);
	
	if(n == NULL){
		printf("no such key\n");
	}else{
		// use qsort (fast) to sort elements in-place.
		qsort(n->values, n->length, sizeof(int), sortcmp);
		// output
		printf("ok\n");
	}
};

void command_diff(){
	// 1) symmetric difference: union minus intersection
	// 2) Postgres: binary left groups
	// 3) set difference (correct): binary right groupings
	int * list1 = malloc(1*sizeof(int));
	int * list2 = malloc(1*sizeof(int));
	entry* first_set;
	entry* second_set;
	array* arr1 = malloc(sizeof(array));
	array* arr2 = malloc(sizeof(array));
	array* rtn;
		
	// loop over all argv[]
	int position = 0;
	while(argv[COMMAND_KEY_NUM+position+1] != NULL){
		if(position == 0){
			first_set = entry_find(entry_head, argv[COMMAND_KEY_NUM]);
		}
		
		second_set = entry_find(entry_head, argv[COMMAND_KEY_NUM+position+1]);
		
		if(first_set == NULL || second_set == NULL){
			printf("no such key\n");
			return;
		}else{
			// load up arrays
			
			if(position == 0){
				// initial run, there is not list...so load up from entry
				list1 = realloc(list1, first_set->length*sizeof(int));
				memcpy(list1, first_set->values, first_set->length*sizeof(int));
				arr1->values = list1;
				arr1->length = first_set->length;
				// to skip over one (since we don't need to compare the second again...)
				//position++;
				// sort and uniq
				qsort(arr1->values, arr1->length, sizeof(int), sortcmp);
				arr1->length = uniq(arr1->values, arr1->length);
				arr1->values = realloc(arr1->values, arr1->length*sizeof(int));
			}else{
				// continue using previous list...	
				
			}
			free(list2);
			list2 = malloc(second_set->length*sizeof(int));
			
			memcpy(list2, second_set->values, second_set->length*sizeof(int));
			arr2->values = list2;
			arr2->length = second_set->length;
			
			// sort and uniq
			qsort(arr2->values, arr2->length, sizeof(int), sortcmp);
			arr2->length = uniq(arr2->values, arr2->length);
			arr2->values = realloc(arr2->values, arr2->length*sizeof(int));
			
			// send to function
			rtn = difference(arr1, arr2);
			
			// set arr1 to rtn
			memcpy(arr1->values, rtn->values, rtn->length*sizeof(int));
			arr1->length = rtn->length;
			
			// free return
			free(rtn->values);
			free(rtn);
			
			
			// increment position
			position++;
		}
	}

	printf("[");
	// ... in the accepted format
	for(int i = 0; i < arr1->length; i++){
		if(i == arr1->length-1){
			printf("%d", arr1->values[i]);
		}else{
			printf("%d ", arr1->values[i]);
		}
	}
	printf("]\n");
	
	
	// free structs (no longer needed)
	free(arr1->values);
	free(arr2->values);
	free(arr1);
	free(arr2);
};

void command_inter(){
	int * list1 = malloc(1*sizeof(int));
	int * list2 = malloc(1*sizeof(int));
	entry* first_set;
	entry* second_set;
	array* arr1 = malloc(sizeof(array));
	array* arr2 = malloc(sizeof(array));
	array* rtn;
	
	// loop over all argv[]
	int position = 0;
	while(argv[COMMAND_KEY_NUM+position+1] != NULL){
		if(position == 0){
			first_set = entry_find(entry_head, argv[COMMAND_KEY_NUM]);
		}
		
		second_set = entry_find(entry_head, argv[COMMAND_KEY_NUM+position+1]);
		
		if(first_set == NULL || second_set == NULL){
			printf("no such key\n");
			return;
		}else{
			// load up arrays
			
			if(position == 0){
				// initial run, there is not list...so load up from entry
				list1 = realloc(list1, first_set->length*sizeof(int));
				memcpy(list1, first_set->values, first_set->length*sizeof(int));
				arr1->values = list1;
				arr1->length = first_set->length;
				// to skip over one (since we don't need to compare the second again...)
				//position++;
				// sort and uniq
				qsort(arr1->values, arr1->length, sizeof(int), sortcmp);
				arr1->length = uniq(arr1->values, arr1->length);
				arr1->values = realloc(arr1->values, arr1->length*sizeof(int));
			}else{
				// continue using previous list...	
				
			}
			
			free(list2);
			list2 = (int *)malloc((second_set->length)*sizeof(int));	

			memcpy(list2, second_set->values, second_set->length*sizeof(int));
			arr2->values = list2;
			arr2->length = second_set->length;
			
			// sort and uniq
			qsort(arr2->values, arr2->length, sizeof(int), sortcmp);
			arr2->length = uniq(arr2->values, arr2->length);
			arr2->values = realloc(arr2->values, arr2->length*sizeof(int));
			
			// send to function
			rtn = intersection(arr1, arr2);
			
			// set arr1 to rtn
			memcpy(arr1->values, rtn->values, rtn->length*sizeof(int));
			arr1->length = rtn->length;
			
			// free return
			free(rtn->values);
			free(rtn);
			
			
			// increment position
			position++;
		}
	}

	printf("[");
	// ... in the accepted format
	for(int i = 0; i < arr1->length; i++){
		if(i == arr1->length-1){
			printf("%d", arr1->values[i]);
		}else{
			printf("%d ", arr1->values[i]);
		}
	}
	printf("]\n");
	
	
	// free structs (no longer needed)
	free(arr1->values);
	free(arr2->values);
	free(arr1);
	free(arr2);
};

void command_union(){
	int * list = malloc(1*sizeof(int));
	entry* n;
	
	int index = 0;
	// search for n-th entry with given <key>
	for(int i = 0; argv[COMMAND_KEY_NUM+i] != NULL; i++){
		n = entry_find(entry_head, argv[COMMAND_KEY_NUM+i]);
		
		// check if entry exists:
		if(n == NULL) {
			printf("no such key\n");
			return;
		}
		// add items to list:
		for(int j = 0; j < n->length; j++){
			list[index] = n->values[j];
			index++;
			list = (int *)realloc(list, (index+1)*sizeof(int));
		}
	}

	// sort entries
	qsort(list, index, sizeof(int), sortcmp);
	
	index = uniq(list, index);
	
	list = realloc(list, index*sizeof(int));

	// output the list
	printf("[");
	// ... in the accepted format
	for(int i = 0; i < index; i++){
		if(i == index-1){
			printf("%d", list[i]);
		}else{
			printf("%d ", list[i]);
		}
	}
	printf("]\n");
	
	// free list;
	free(list);
};

void command_listKeys(){
	if(entry_head == NULL){
		// the list is empty
		printf("no keys\n");
	}else{
		// loop through all entries in the database (entry_head)
		entry* current = entry_head;
		
		while(current != NULL){
			// output each key
			printf("%s\n", current->key);
			current = current->next;
		}
	}
};

void command_listEntries(){
	if(entry_head==NULL){
		// the list is empty
		printf("no entries\n");
	}else{
		// loop through all entries in the database (entry_head)
		entry* current = entry_head;
		while(current != NULL){
			// output the <key>
			printf("%s [", current->key);
			
			// ...then the entries in the accepted format
			for(int i = 0; i < current->length; i++){
				if(i==(current->length)-1){
					printf("%d", current->values[i]);
				}else{
					printf("%d ", current->values[i]);
				}
			}
			printf("]\n");
			// move to next entry
			current = current->next;
		}
	}
};

void command_listSnapshots(){
	if(snapshot_head==NULL){
		// the snapshots list is empty
		printf("no snapshots\n");
	}else{
		// loop through all snapshots (snapshot_head)
		snapshot* current = snapshot_head;
		while(current != NULL){
			// output each <id>
			printf("%d\n", current->id);
			// move to next snapshot
			current = current->next;
		}
	}
};

/*
 *	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *	HELPER FUNCTIONS FOR ACCESSING LISTS
 *	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

 /*
  *	Description: 	Searches given entry for a value
  *	Return:			value if found, NULL if not found
  */
// int entry_searchcmp(entry* first, entry* second, int search){
	// int isInFirst = 0;
	// int isInSecond = 0;
	// for(int i = 0; i < first->length; i++){
		// if(first->values[i] == search) isInFirst = 1;
	// }
	
	// if(isInFirst == 0) return NULL;
	
	// for(int i = 0; i < second->length; i++){
		// if(second->values[i] == search) isInSecond = 1;
	// }
	
	// if(isInFirst && isInSecond) return value;
	// else return NULL;
// }

// int entry_search(entry* first, int search){
	// for(int i = 0; i < first->length; i++){
		// if(first->values[i] == search) return value;
	// }
	// return NULL;
// }

array* intersection(array* first, array* second){
	// returns common elements.
	array* object = malloc(sizeof(array));
	int * list = malloc(1*sizeof(int));
	int term;
	int index = 0;
	
	for(int j = 0; j < first->length; j++){
		term = first->values[j];
		
		for(int i = 0; i < second->length; i++){
			if(second->values[i] == term){
				// is in both...
				list[index] = term;
				index++;
				list = (int *)realloc(list, (index+1)*sizeof(int));
				break;
			}
		}
	}
	// clean data
	qsort(list, index, sizeof(int), sortcmp);
	index = uniq(list,index);
	list = realloc(list, index*sizeof(int));
	//for(int i = 0; i < index; i++) printf("line 860: %d", list[i]);
	object->values = list;
	object->length = index;
	return object;
}

array* difference(array* first, array* second){
	// data is sorted and unique
	// returns common elements.
	array* object = malloc(sizeof(array));
	int * list = malloc(first->length*sizeof(int));
	memcpy(list, first->values, first->length*sizeof(int));
	int term;
	int index = first->length;
	
	for(int j = 0; j < second->length; j++){
		term = second->values[j];
		
		for(int i = 0; i < second->length; i++){
			if(list[i] == term){
				// REMOVE FROM LIST by shifting left
				// shift all values left by one slot.
				for(int c = i; c < index-1; c++) list[c] = list[c+1];
				index--;
				break;
			}
		}
	}
	// clean data
	qsort(list, index, sizeof(int), sortcmp);
	index = uniq(list,index);
	list = realloc(list, index*sizeof(int));
	//for(int i = 0; i < index; i++) printf("line 860: %d", list[i]);
	object->values = list;
	object->length = index;
	return object;
}
 
 /*
  *	Description: 	Removes all adjacent duplicate values
  *	Return:			Pointer to list that was changed
  */
 int uniq(int* values, int length){
	// check if element is same, side by side.
	for(int j = 1; j < length; j++){
		if(values[j] == values[j-1]){
			// shift all values left by one slot.
			for(int i = j; i < length-1; i++) values[i] = values[i+1];
			
			// resize array
			//values = realloc(values, (length-1)*sizeof(int));
			
			// update length
			length--;
			
			// check these again, since everything moved.
			j--;
		}
	}
	return length;
 }
 
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
entry* entry_create(void){
	entry* n = (entry *) malloc(sizeof(entry));
	int length = 0;
	
	// building list for entry:	
	int* list = (int *)malloc(1 * sizeof(int));
	
	if(argv[COMMAND_VALUES_INITAL] == NULL){
		// empty list
	}else if(argv[COMMAND_VALUES_INITAL+1] == NULL){
		// one item in list
		list[0] = atoi(argv[COMMAND_VALUES_INITAL]);
		length = 1;
	}else{
		// multiple items:
		length = 0;

		for(; argv[COMMAND_VALUES_INITAL+length] != NULL; length++){
			list = realloc(list, (length+1)*sizeof(int));
			list[length] = atoi(argv[COMMAND_VALUES_INITAL+length]);
		}

	}
	
	// check key length:
	if(strlen(argv[COMMAND_KEY_NUM])>MAX_KEY){
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
	
	for(; argv[COMMAND_VALUES_INITAL+length] != NULL; length++){
			list = realloc(list, (length+1)*sizeof(int));
			list[length] = atoi(argv[COMMAND_VALUES_INITAL+length]);
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
void entry_append(entry* n){
	entry* current = entry_head;
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
void entry_push(entry* n){
	entry* current = entry_head;
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
	
	// rebuild links:
	if(n->next == NULL && n->prev == NULL){
		// only element
	}
	// first element
	else if(n->prev == NULL && n->next != NULL) n->next->prev = NULL;
	else if(n->prev != NULL && n->next != NULL){
		// middle element
		n->prev->next = n->next;
		n->next->prev = n->prev;
	}
	// last element
	else if(n->prev != NULL && n->next == NULL) n->prev->next = NULL;

	
	// free memory of values, then the struct
	free(n->values);
	free(n);
	// return next element
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
void snapshot_append(snapshot* n){
	snapshot* current = snapshot_head;
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
 *	Description: 	Adds provided snapshot to beginning of linked-list (UNSAFE).
 *	Return: 		none.
 */
void snapshot_push(snapshot* n){
	
	if(snapshot_head == NULL){
		// first element in list.
		snapshot_head = n;
		snapshot_tail = n;
		n->next = NULL;
		n->prev = NULL;
	}else{	
		// create link
		snapshot_head->prev = n;
		n->next = snapshot_head;
		n->prev = NULL;
		// for rapid fire adding of elements:
		snapshot_head = n;
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
 *					 NOTE: This method is unsafe because it calls snapshot_remove()
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
