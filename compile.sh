#!/bin/bash
gcc -Wall -Werror -std=c11 snapshot.c -o snapshot
#-fsanitize=address