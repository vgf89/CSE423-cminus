#
# CSE423 Fall 2017
# Group: _Za_Worldo_
#

BIN = parser
OUTPUT = c-
CC = gcc
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

printtree.c: printtree.h scanType.h treenode.h
	$(CC) $(CFLAGS) -c printtree.h -o printtree.o

annotated.c: annotated.h treenode.h printtree.h
	$(CC) $(CFLAGS) -c annotated.h -o annotated.o

all:
	make

clean:
	rm -f $(OBJS) lex.yy.c $(BIN).tab.h $(BIN).tab.c $(BIN).tar $(BIN).output $(OUTPUT)

tar:
	tar -cvf $(BIN).tar $(SRCS) makefile
