#!/usr/bin/env bash
rm -rf lex.yy.c *.tab.* 2>/dev/null

flex fb.l
bison -d fb.y
gcc-13 -mmacosx-version-min=13.3 -ll -o fb fb.tab.c lex.yy.c 
./fb $@

# In Unix-like operating systems, file descriptors are used to represent input, output, and error streams. 
# By default, file descriptor 2 is associated with the standard error (stderr), which is used to output error 
# messages.

# The 2>/dev/null part of a command is a redirection operator that redirects the output of file descriptor 2 
# (stderr) to /dev/null. In Unix-like systems, /dev/null is a special device file that discards any data written 
# to it and provides no data when read from it.