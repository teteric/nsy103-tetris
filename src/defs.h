#ifndef __DEFS_H__
#define __DEFS_H__


#define BOARD_WIDTH  12
#define BOARD_HEIGHT 25
#define BOARD_DRAW_OFFSET 5

#define BLOCK_WIDTH 4
#define BLOCK_HEIGHT 4

#define NET_BUFFER_LENGTH 1024
#define IP_STRING_SIZE 16+1

#define DEFAULT_SERVER_IP "127.0.0.1"
#define DEFAULT_SERVER_PORT 3000

#define VERSION "0.1.0"

#define LOCALE_KEY_ESCAPE 27

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <fcntl.h>

int done;
char game_server_ip[IP_STRING_SIZE];
int game_server_port;
unsigned int points;
unsigned int debug;
unsigned int auto_mode;


#endif
