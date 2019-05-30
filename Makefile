SRC_DIR   = ./src
OBJ_DIR   = ./obj
INC_DIR   = ./include
PROGRAMS  = udpclient udpserver

CC        = g++
CFLAGS    = -Wall -I. -I$(INC_DIR)

.PHONY: all build clean

all: build

build: $(PROGRAMS)

udpclient: $(OBJ_DIR)/udpclient.o
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/udpclient.o: $(SRC_DIR)/udpclient.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $^ -o $@

udpserver: $(OBJ_DIR)/udpserver.o
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/udpserver.o: $(SRC_DIR)/udpserver.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f $(PROGRAMS)
	rm -rf $(OBJ_DIR)
