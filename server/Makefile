
.PHONY: all clean clean_all server
all: clean server

CFILES =  server.c configHandler.c
APP=server
CC=gcc
CFLAGS=-o

clean:
	@echo "Cleaning files..."
	@rm -f *.out
	@rm -f *.o
	@rm -f server

clean_all: 
	@echo "Cleaning ALL files..."
	@rm -f *.out
	@rm -f *.o
	@rm -rf logs/ 
	@rm -rf cols
	@rm -rf his/

server:
	@echo "Creating program..."
	$(CC) $(CFILES) $(CFLAGS) $(APP)

