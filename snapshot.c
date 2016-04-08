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



void command_bye () {
    printf("bye\n");
}

void command_help() {
    printf("%s\n", HELP);
}


int (*ptrc_bye)();
int (*ptrc_help)();

ptrc_bye = &command_bye;
ptrc_help = &command_help;

int* COMMAND_FUNC[] = {
	ptrc_bye, ptrc_help,
	
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
    	    command_bye();
    	    return 0; 		
    	}
		else{
			char cmd_input[9]; // longest command is 9
			int nargs;
			// check for LIST commands:
			nargs = sscanf(line, " %s", cmd_input);
			if(nargs){
			}
		}
			
    	
	    //
	    // TODO
	    //
  	}

	return 0;
}