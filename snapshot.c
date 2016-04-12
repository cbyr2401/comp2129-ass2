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

int list_size(entry* head){
	if(head==NULL){
		return 0;
	}else{
		entry* current = head;
		int size = 0;
		while(current != NULL){
			current = head->next;
			size++;
		}
		return size;
	}
}

void list_keys(entry * head){
	if(head==NULL){
		// no keys:
		printf("no keys\n");
	}else{
		entry* current = head;
		while(current != NULL){
			printf("%s\n", current->key);
			current = head->next;
		}
	}
}

void list_entries(entry* head){
	if(head==NULL){
		// no entries:
		printf("no entries\n");
	}else{
		entry* current = head;
		while(current != NULL){
			// TODO:  check if I can just print the int array without going through it.
			printf("%s [", current->key);
			for(int i = 0; i < sizeof(current->values)/sizeof(int); i++){
				printf("%d", current->values[i]);
			}
			printf("]\n");
			current = head->next;
		}
	}
}

void list_snapshots(snapshot* head){
	if(head==NULL){
		// no snapshots:
		printf("no snapshots\n");
	}else{
		snapshot* current = head;
		while(current != NULL){
			printf("%d\n", current->id);
			current = head->next;
		}
	}
}

void list_appendE(entry* head, char* key, int* values){
	if(head==NULL){
		// no elements:
		entry* new = malloc(sizeof(entry));
		strncpy(new->key,key,sizeof(key)/sizeof(char));
		new->length = sizeof(values)/sizeof(int);
		new->values = create_values(values);
		new->prev = NULL;
		new->next = NULL;
		entry_head = new;
	}else{
		// some elements
		// TODO: implement with entry tail...
		//entry* last = entry_tail;

		entry* current = head;
		while(current->next != NULL){
			current = current->next;
		}
		entry* new = malloc(sizeof(entry));
		strncpy(new->key,key,sizeof(key)/sizeof(char));
		new->length = sizeof(values)/sizeof(int);
		new->values = create_values(values);
		new->prev = current;
		new->next = NULL;
	}
}

void list_deleteE(entry* head, char* ke){
	
}



int main(void) {

	char line[MAX_LINE];

	while (true) {
    	printf("> ");
		// get input from user and check it is valid
		//input = fgets(buffer, 12, stdin);
		// check for null
		//if(input=='\0') CleanExitError();
		// parse
		//nargs = sscanf(input, " %s %c", &cmd, &x, &y, extra);

    	if (fgets(line, MAX_LINE, stdin) == NULL) {
    	    //(*ptrc[0])();
    	    return 0; 		
    	}
		else{
			char * argv[MAX_COMMAND];
			char *word;
			const char delim[2] = " ";
			//char cmd[9]; // longest command is 9
			int nargs;
			// check for LIST commands:

			//nargs = sscanf(line, " %s", cmd);
			
			nargs = 0;
			argv[nargs] = strtok(line, delim);
			
			// sourced from: https://gist.github.com/efeciftci/9120921
			while((word = strtok(NULL, delim)) != NULL){
				nargs++;
				argv[nargs] = word;
			}
			
			if(nargs < 1){
				
			}
			
			// convert all characters for command to uppercase for search...
			for(int c=0; c < strlen(argv[0]); c++) argv[0][c] = toupper(argv[0][c]);
			
			// check for "LIST"
			if(strcmp(argv[0], "LIST")==0){
				// convert second word to upper case:
				for(int c=0; c < strlen(argv[1]); c++) argv[1][c] = toupper(argv[1][c]);
				// concat the two words together:
				//strcat(strcat(argv[0], delim), argv[1]);
				argv[0][strlen(argv[0])] = ' ';  // replaces '\0' with ' ', bit dangerous.
			}
			// TODO: strcat returning that there is more in argv[0] than there should be.
			printf("%d", strcmp(argv[0], "LIST SNAPSHOTS"));
			
			// look for a matching command and then run it.
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
	list_entries(entry_head);
};
void command_listSnapshots(){
	list_snapshots(snapshot_head);
};