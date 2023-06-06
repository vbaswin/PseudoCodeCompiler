#!/usr/bin/env	bash

rm -rf obj/* bin/* 2>/dev/null

flex -o obj/lex.yy.c src/fb.l 
bison -d src/fb.y -o obj/fb.tab.c
gcc-13 -mmacosx-version-min=13.3 -ll -o bin/fb obj/fb.tab.c obj/lex.yy.c src/fb.c src/symtab.c
bin/fb $@
