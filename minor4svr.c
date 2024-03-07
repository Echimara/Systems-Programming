// Program Description: "minor4svr.c" accepts one argument (a valid port no.),
// listens for PING messages from clients and sends back a PONG msg
// necessary header files for sockets, system calls, and flags (MSG_WAITALL & MSG_CONFIRM)

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>
#include <time.h>
// define buffer size used in recv() function call
#define MAX 1024
int main(int argc, char* argv[])
{
// validate argument input of ./minor4svr <port>
if (argc != 2)
{
printf("usage: %s <port>\n", argv[0]);
// terminate program
exit(EXIT_FAILURE);
}
// generation of a seeded, randomized integer
srand(time(NULL));
int sockfd; // socket fd
int cli_size; // address size of client
int portno; // port no.

int n; // no. of bytes accepted
struct sockaddr_in serv_addr; // serv_addr - made up of the socket address and port no.
struct sockaddr_in cli_addr; // client address
char* pong = "PONG"; // points to "PONG" and print "PONG" on server-side when svr recieves a ping
msg
char sendBuff[MAX]; // to store sent data

// creates a socket fd
if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
{
printf("socket error\n");
exit(EXIT_FAILURE);
}
// initialize memory to zero
memset(&serv_addr, '0', sizeof(serv_addr));
// initliaze memory to zero
memset(sendBuff, '0', sizeof(sendBuff));
// specify server attributes
serv_addr.sin_family = AF_INET;
serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
portno = atoi(argv[1]);
serv_addr.sin_port = htons(portno);

// binds the socket with a server address
if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) // Bind socket with the server
address
{
printf("bind error\n");
exit(EXIT_FAILURE);
}
printf("[server]: ready to accept data...\n");
while (1)
{
// getter - get client address size
cli_size = sizeof(cli_addr);
// receive ping message from client

n = recvfrom(sockfd, (char*)sendBuff, MAX, MSG_WAITALL, (struct sockaddr*)&cli_addr,

&cli_size); // Receive ping message from client

if (n < 0) // if msg was lost or failed to be recieved
{
printf("[server]: dropped packet\n"); // If the message is lost, print an appropriate

message to the terminal
continue;
}
if (rand() % 10 < 3) // simulate 30% packet loss
{
printf("[server]: dropped packet\n"); // msg was lost or failed to be recieved
continue;
}
sendBuff[n] = 0;
// print 'PING' msg
printf("[client]: %s\n", sendBuff);
// Send PONG message back to the client
n = sendto(sockfd, (const char*)pong, strlen(pong), MSG_CONFIRM, (const struct

sockaddr*)&cli_addr, cli_size);
if (n < 0)
{
printf("[server]: dropped packet\n"); // msg was lost or failed to be recieved
continue;
}
// print each 'PONG' msg sent to client
printf("[server]: PONG\n");
}
return 0;
}
