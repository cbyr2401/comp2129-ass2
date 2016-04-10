#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#define MAX_KEY 16
#define MAX_LINE 1024
#define MAX_COMMAND 32

typedef struct entry entry;
typedef struct snapshot snapshot;

struct entry {
	char key[MAX_KEY];
	int* values;
	size_t length;
	entry* next;
	entry* prev;
};

struct snapshot {
	int id;
	entry* entries;
	snapshot* next;
	snapshot* prev;
};

const char* HELP =
	"BYE   clear database and exit\n"
	"HELP  display this help message\n"
	"\n"
	"LIST KEYS       displays all keys in current state\n"
	"LIST ENTRIES    displays all entries in current state\n"
	"LIST SNAPSHOTS  displays all snapshots in the database\n"
	"\n"
	"GET <key>    displays entry values\n"
	"DEL <key>    deletes entry from current state\n"
	"PURGE <key>  deletes entry from current state and snapshots\n"
	"\n"
	"SET <key> <value ...>     sets entry values\n"
	"PUSH <key> <value ...>    pushes values to the front\n"
	"APPEND <key> <value ...>  appends values to the back\n"
	"\n"
	"PICK <key> <index>   displays value at index\n"
	"PLUCK <key> <index>  displays and removes value at index\n"
	"POP <key>            displays and removes the front value\n"
	"\n"
	"DROP <id>      deletes snapshot\n"
	"ROLLBACK <id>  restores to snapshot and deletes newer snapshots\n"
	"CHECKOUT <id>  replaces current state with a copy of snapshot\n"
	"SNAPSHOT       saves the current state as a snapshot\n"
	"\n"
	"MIN <key>  displays minimum value\n"
	"MAX <key>  displays maximum value\n"
	"SUM <key>  displays sum of values\n"
	"LEN <key>  displays number of values\n"
	"\n"
	"REV <key>   reverses order of values\n"
	"UNIQ <key>  removes repeated adjacent values\n"
	"SORT <key>  sorts values in ascending order\n"
	"\n"
	"DIFF <key> <key ...>   displays set difference of values in keys\n"
	"INTER <key> <key ...>  displays set intersection of values in keys\n"
	"UNION <key> <key ...>  displays set union of values in keys\n";

const char* COMMAND_LIST[] = {
	"BYE", "HELP",
	"GET", "DEL", "PURGE",
	"SET", "PUSH", "APPEND",
	"PICK", "PLUCK", "POP",
	"DROP", "ROLLBACK", "CHECKOUT", "SNAPSHOT",
	"MIN", "MAX", "SUM", "LEN",
	"REV", "UNIQ", "SORT", 
	"DIFF", "INTER", "UNION",
	"LIST KEYS", "LIST ENTRIES", "LIST SNAPSHOTS"
};

/*
	Function Declarations
	 * Each of these will call a sub-function... these allow for
	    fast searching of commands.
*/
void command_bye(void);
void command_help(void);

void command_listKeys(void);
void command_listEntries(void);
void command_listSnapshots(void);

void command_get(void);
void command_del(void);
void command_purge(void);

void command_set(void);
void command_push(void);
void command_append(void);

void command_pick(void);
void command_pluck(void);
void command_pop(void);

void command_drop(void);
void command_rollback(void);
void command_checkout(void);
void command_snapshot(void);

void command_min(void);
void command_max(void);
void command_sum(void);
void command_len(void);

void command_rev(void);
void command_uniq(void);
void command_sort(void);

void command_diff(void);
void command_inter(void);
void command_union(void);

#endif