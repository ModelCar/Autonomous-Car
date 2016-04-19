/* A simple blocking server in the internet domain using TCP */
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;


int serversocketfd;

struct driveinfo{
	int speed;
	bool stop;
};

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int netSendInfo(int socketfd,driveinfo *info){
		uint16_t net[2];
	net[0]=htons(info->speed);
	net[1]=htons(info->stop);

	return write(socketfd,(char*)&net,sizeof(net));
}

int netReceiveInfo(int newsockfd,driveinfo *info){
	uint16_t net[2];
	int n = read(newsockfd,&net,sizeof(net));
	if (n < 2) error("ERROR reading from socket");
	info->speed = ntohs(net[0]);
	info->stop = ntohs(net[1]);
}

void serverCleanup(){
	close(serversocketfd);
}

int netCreateSingleBlockingServer(int port, const char* message = "Server initialised, waiting connection"){
	struct sockaddr_in serv_addr, cli_addr;
	int  newsockfd;
	socklen_t clilen;
			
		// AF_INET for Network, SOCK_STREAM for TCP
	serversocketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (serversocketfd < 0) 
		error("ERROR opening socket");
	// Allow reuse of the port, else its lingering up to 4 Minutes after its closed...
	int optionvalue=1;
	setsockopt(serversocketfd,SOL_SOCKET,(SO_REUSEPORT | SO_REUSEADDR),&optionvalue,sizeof(optionvalue));
	// define socket parameters
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	//     htons converts to network byte order "host to networkorder short"
	serv_addr.sin_port = htons(port);
	if (bind(serversocketfd, (struct sockaddr *) &serv_addr,
		sizeof(serv_addr)) < 0) 
		error("ERROR on binding");
	cout << message << endl;
	// listen on this socket and allow a max of 5 pending conenctions
	listen(serversocketfd,5);
	//  block till someone connects and return the socketfd for this cnnection
	newsockfd = accept(serversocketfd, 
										 (struct sockaddr *) &cli_addr, 
										 &clilen);
	if (newsockfd < 0) 
		error("ERROR on accept");
	// clean up on exit, is this possible without a global var? 
	atexit(&serverCleanup);
	// return new socketfd so main prog can write to it.
	return newsockfd;
}



int b_main(int argc, char *argv[])
{
	char buffer[256];
	int n,newsockfd;
	
	driveinfo info;

	newsockfd = netCreateSingleBlockingServer(8000);
	netReceiveInfo(newsockfd,&info);
	printf("Here is the message: speed=%i, stop=%i\n",info.speed,info.stop);
	netReceiveInfo(newsockfd,&info);
	printf("Here is the message: speed=%i, stop=%i\n",info.speed,info.stop);
	
	n = write(newsockfd,"I got your message",18);
	if (n < 0) error("ERROR writing to socket");
	close(newsockfd);
	return 0; 
}