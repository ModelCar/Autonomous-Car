#ifndef NETWORK_CLIENT_H
#define NETWORK_CLIENT_H


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 


struct driveinfo{
	int speed;
	bool stop;
	int trafficlight; // 1 green, 2 = yellow, 3 = red
	bool rightofway;
	bool attention;
};

void client_error(const char *msg);

int netSend(int socketfd,driveinfo *info);

int netReceiveInfo(int sockfd,driveinfo *info);

void clientCleanup();

int netConnect(const char *serveraddr,int portnumber);

#endif