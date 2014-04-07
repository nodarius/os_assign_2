CC=gcc
CFLAGS=-Wall -g 
EXECUTABLE=calc
LINK=-lrt
SRC=master.c 
SRC1=child.c
EX1=child
all: $(EXECUTABLE)

$(EXECUTABLE): $(SRC) $(SRC1)
	$(CC) $(CFLAGS) $(SRC) -o $(EXECUTABLE) $(LINK)
	$(CC) $(CFLAGS) $(SRC1) -o $(EX1) $(LINK)
run:
	./$(EXECUTABLE)
clean:
	rm -f $(EXECUTABLE) *~ 
	rm -f $(EXECUTABLE) *.o
	rm -f child *.o
	rm -f child *~


