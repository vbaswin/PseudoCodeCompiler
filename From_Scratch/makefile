CC = gcc
CFLAGS := -mmacosx-version-min=13.3 -ll

# folders
BDIR = bin
IDIR = inc
SDIR = src
ODIR = obj
OUTDIR = output

_BIN = fb
BIN = $(patsubst %, $(BDIR)/%, $(_BIN))

_OBJ = lex.yy.c fb.tab.c
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

_SRC = ast.c cwd.c eval.c fb.c fb.l fb.y intermediate.c symtab.c
SRC = $(patsubst %, $(SDIR)/%, $(_SRC))

default: exec

obj/lex.yy.c: src/fb.l
	flex -o obj/lex.yy.c src/fb.l 

obj/fb.tab.c: src/fb.y
	bison -o obj/fb.tab.c -d src/fb.y 

exec: $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ) $(SRC)


.PHONY: clean
clean:
	rm -rf $(ODIR)/* $(BDIR)/* $(OUTDIR)/*
