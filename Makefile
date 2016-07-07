CC = gcc
OBJ_NAME = gss
OBJS = Main.c loadConfig.c
FLAGS = -Wall -lcurl

all:
	$(CC) $(FLAGS) $(OBJS) -o $(OBJ_NAME)
clean:
	rm $(OBJ_NAME)

