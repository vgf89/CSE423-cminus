BIN = parser
OUTPUT = c-
CC = gcc
CFLAGS = -g

SRCS = $(BIN).l $(BIN).y tree.h scanType.h
OBJS = lex.yy.o $(BIN).tab.o tree.o
LIBS = -lfl


$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(OUTPUT)

$(BIN).tab.h $(BIN).tab.c: $(BIN).y
	bison -v -t -d $(BIN).y

lex.yy.c: $(BIN).l $(BIN).tab.h
	flex $(BIN).l

tree.c: tree.h scanType.h
	$(CC) $(CFLAGS) -c tree.h

all:
	make

clean:
	rm -f $(OBJS) lex.yy.c $(BIN).tab.h $(BIN).tab.c $(BIN).tar $(BIN).output $(OUTPUT)

tar:
	tar -cvf $(BIN).tar $(SRCS) makefile
