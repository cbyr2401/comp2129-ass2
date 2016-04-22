#!/usr/bin/env bash
gcc -Wall -Werror -std=c11 snapshot.c -o snapshot
#-fsanitize=address