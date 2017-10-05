#
# CSE423 Fall 2017
# Group: _Za_Worldo_
#

BIN = parser
OUTPUT = c-
CC = g++
CFLAGS = -g

SRCS = $(BIN).l $(BIN).y printtree.h annotated.h scanType.h treenode.h
OBJS = lex.yy.o $(BIN).tab.o printtree.o annotated.o
LIBS = -lfl


$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(OUTPUT)

$(BIN).tab.h $(BIN).tab.c: $(BIN).y
	bison -v -t -d $(BIN).y

lex.yy.c: $(BIN).l $(BIN).tab.h
	flex $(BIN).l

printtree.c: printtree.h printtree.c
	$(CC) $(CFLAGS) -c printtree.c -o printtree.o

annotated.c: annotated.c annotated.h
	$(CC) $(CFLAGS) -c annotated.c -o annotated.o

all:
	make

clean:
	rm -f $(OBJS) lex.yy.c $(BIN).tab.h $(BIN).tab.c $(BIN).tar $(BIN).output $(OUTPUT)

tar:
	tar -cvf $(BIN).tar $(SRCS) makefile
