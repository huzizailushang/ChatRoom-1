OBJS = main.o ChatManager.o SocketManager.o
CC = g++
SRC_DIR = ./server/src/
INC_DIR = ./server/include/
OBJ_DIR = ./objs/
INCLUDE = -I $(SRC_DIR) -I $(INC_DIR) -I $(OBJ_DIR)

vpath %.h $(INC_DIR)
vpath %.cpp $(SRC_DIR)
vpath %.o $(OBJ_DIR)

server : main.o SocketManager.o ChatManager.o main.o
	$(CC) $(addprefix $(OBJ_DIR), $(OBJS)) -o server.o 
	
main.o : main.cpp SocketManager.h ChatManager.h
	$(CC) -c $< $(INCLUDE) -o $(OBJ_DIR)$@

SocketManager.o : SocketManager.cpp SocketManager.h 
	$(CC) -c $< $(INCLUDE) -o $(OBJ_DIR)$@

ChatManager.o : ChatManager.cpp ChatManager.h global.h
	$(CC) -c $< $(INCLUDE) -o $(OBJ_DIR)$@

clean:
	-rm $(OBJ_DIR)*.o server.o