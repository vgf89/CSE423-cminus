BIN = parser
CC = gcc
CFLAGS = -g

#SRCS = $(BIN).l $(BIN).y
SRCS = $(BIN).l
#OBJS = lex.yy.o $(BIN).tab.o
OBJS = lex.yy.o
LIBS = -lfl


$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(BIN)

#$(BIN).tab.h $(BIN).tab.c: $(BIN).y
#	bison -v -t -d $(BIN).y

lex.yy.c: $(BIN).l# $(BIN).tab.h
	flex $(BIN).l

all:
	make

clean:
	rm -f $(OBJS) lex.yy.c $(BIN).tab.h $(BIN).tab.c $(BIN).tar $(BIN).output $(BIN)

tar:
	tar -cvf $(BIN).tar $(SRCS) makefile