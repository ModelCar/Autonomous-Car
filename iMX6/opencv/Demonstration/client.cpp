#include "client.h"

int clientsocketfd;

void client_error(const char *msg)
{
	perror(msg);
	exit(0);
}

// int netSend(int socketfd,driveinfo *info){
// 		uint16_t net[2];
// 	net[0]=htons(info->speed);
// 	net[1]=htons(info->stop);
// 
// 	return write(socketfd,(char*)&net,sizeof(net));
// }

int netReceiveInfo(int sockfd,driveinfo *info){
	uint16_t net[5];
	int n = read(sockfd,&net,sizeof(net));
	if (n < 2) client_error("ERROR reading from socket");
	info->speed = ntohs(net[0]);
	info->stop = ntohs(net[1]);
	info->trafficlight = ntohs(net[2]);
	info->rightofway = ntohs(net[3]);
	info->attention = ntohs(net[4]);
}

void clientCleanup(){
	close(clientsocketfd);
}

int netConnect(const char *serveraddr,int portnumber){
		int sockfd;
		struct sockaddr_in serv_addr;
		struct hostent *server;
			
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		client_error("ERROR opening socket");
	server = gethostbyname(serveraddr);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, 
				(char *)&serv_addr.sin_addr.s_addr,
				server->h_length);
	serv_addr.sin_port = htons(portnumber);
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
		client_error("ERROR connecting");

	// clean up on exit, is this possible without a global var? 
	atexit(&clientCleanup);
	clientsocketfd=sockfd;
	return sockfd;
}

// int main(int argc, char *argv[])
// {
// 	int n, sockfd;
// 	char buffer[256];
// 	driveinfo info;
// 	
// 	if (argc < 2) {
// 		fprintf(stderr,"usage %s hostname for own server, trying 10.42.0.254\n", argv[0]);
// 		exit(0);
// 	}
// 		
// 	
// 	sockfd = netConnect(argv[1],8000);	
// 	
// 	
// 	
// 	printf("connected, waiting for data: ");
//   while(1) {
// 		netReceiveInfo(sockfd,&info);
// 		printf("Here is the message: speed=%i, stop=%i\n",info.speed,info.stop);
// 		
// 	}
// 	return 0;
// }