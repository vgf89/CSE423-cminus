#
# CSE423 Fall 2017
# Group: _Za_Worldo_
#

BIN = parser
OUTPUT = c-
CC = g++
CFLAGS = -g

SRCS = $(BIN).l $(BIN).y printtree.h annotated.h scanType.h treenode.h symbolTable.h
OBJS = lex.yy.o $(BIN).tab.o printtree.o annotated.o symbolTable.o
LIBS = -lfl


$(OUTPUT): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(OUTPUT)

$(BIN).tab.h $(BIN).tab.c lex.yy.c: $(BIN).y
	bison -v -t -d $(BIN).y

lex.yy.o: $(BIN).l lex.yy.c
	flex $(BIN).l
	g++ -g -c -o lex.yy.o lex.yy.c

printtree.o: printtree.h printtree.c
	$(CC) $(CFLAGS) -c printtree.c -o printtree.o

annotated.o: annotated.c annotated.h
	$(CC) $(CFLAGS) -c annotated.c -o annotated.o

symbolTable.o: symbolTable.cpp symbolTable.h
	$(CC) $(CFLAGS) -c symbolTable.cpp -o symbolTable.o 

all:
	make

clean:
	rm -f $(OBJS) lex.yy.c $(BIN).tab.h $(BIN).tab.c $(BIN).tar $(BIN).output $(OUTPUT)

tar:
	tar -cvf $(BIN).tar $(SRCS) makefile
