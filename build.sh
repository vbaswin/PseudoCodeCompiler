#!/usr/bin/env	bash

rm -rf lex.yy.c *.tab.* 2>/dev/null

flex fb.l
bison -d fb.y
gcc-13 -mmacosx-version-min=13.3 -ll -o fb fb.tab.c lex.yy.c fb.c
./fb $@
