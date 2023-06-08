#!/usr/bin/env	bash

rm -rf obj/* bin/* output/* 2>/dev/null

flex -o obj/lex.yy.c src/fb.l 
bison -o obj/fb.tab.c -d src/fb.y 

gcc-13 -mmacosx-version-min=13.3 -ll -o bin/fb obj/fb.tab.c obj/lex.yy.c src/fb.c src/symtab.c src/ast.c src/eval.c src/intermediate.c src/cwd.c src/lextab.c

bin/fb $@
