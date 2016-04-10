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
			char cmd[9]; // longest command is 9
			int nargs;
			// check for LIST commands:

			nargs = sscanf(line, " %s", cmd);

			if(nargs < 1){
				
			}
			// convert all characters to uppercase for search...
			for(int c=0; c < strlen(cmd); c++) cmd[c] = toupper(cmd[c]);
			
			// look for a matching command and then run it.
			for(int i = 0; i < 25; i++){
				if(strcmp(cmd,COMMAND_LIST[i])==0) (*ptrcommand[i])();
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