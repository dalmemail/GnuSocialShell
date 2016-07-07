CC = gcc
OBJ_NAME = gss
OBJS = Main.c loadConfig.c lib/get_account_info.c lib/gnusocial.c gnusocialshell.c lib/send_status.c
FLAGS = -Wall -lcurl

all:
	$(CC) $(FLAGS) $(OBJS) -o $(OBJ_NAME) $$(pkg-config libcurl --cflags --libs)
clean:
	rm $(OBJ_NAME)

