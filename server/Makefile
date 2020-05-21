
.PHONY: all clean clean_all server
all: clean server

CFILES =  ./serverFiles/main.c ./serverFiles/configHandler.c ./serverFiles/post_manage.c
APP=server
CC=gcc
CFLAGS=-o

COLF=`cat config.conf | grep DirCol | cut -d':' -f 2`
HISF=`cat config.conf | grep DirHis | cut -d':' -f 2`
LOGF=`cat config.conf | grep DirLog | cut -d':' -f 2`


clean:
	@echo "Cleaning files..."
	@rm -f *.out
	@rm -f *.o
	@rm -f server

clean_all: 
	@echo "Cleaning ALL files..."
	@rm -f *.out
	@rm -f *.o	
	@rm -f $(APP)
	@rm -rf $(COLF)
	@rm -rf $(HISF)
	@rm -rf $(LOGF)

server:
	@echo "Creating program..."
	$(CC) $(CFILES) $(CFLAGS) $(APP) -lm

