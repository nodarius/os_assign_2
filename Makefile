CC=gcc
CFLAGS+=-Wall -g 
M_SRC=$(wildcard src/master/*.c) $(wildcard src/master/*.h)
M_OBJS=$(M_SRC:.c=.o)
MASTER=master
C_SRC=$(wildcard src/child/*.c)
C_OBJS=$(C_SRC:.c=.o)
CHILD=child
LINK=-lrt -lpthread

all: $(MASTER) $(CHILD)

$(MASTER): $(M_OBJS)
	$(CC) $(M_OBJS) $(LINK) -o $(MASTER)
$(CHILD): $(C_OBJS)
	$(CC) $(C_OBJS) $(LINK) -o $(CHILD)
run:
	./$(MASTER)
clean:
	rm -f $(MASTER) $(CHILD)

.PHONY: all test t

M_tests: 
	checkmk test/master/test_master.check > test/master/test_master.c
	$(CC) -g -I./ src/master/master.c test/master/test_master.c -lcheck -o test/master/master $(LINK)



M_test:
	./test/master/master

