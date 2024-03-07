// Program Description: "minor4cli.c" accepts two arguments (server and port no.),
// sends 10 PING messages to the specified server and prints the round trip time (RTT)
// if messages are recieved. If not, it prints a "timed out" message
// necessary header files for sockets, system calls, and flags (MSG_WAITALL & MSG_CONFIRM)

#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <ctype.h>

// define buffer size used in recv() function call
#define MAXLINE 1024
// get current time
void currentTime(struct timeval* t1, struct timeval* t2)
{
gettimeofday(t1, NULL);
}

int main(int argc, char* argv[])
{
// validate argument input of ./minor4cli <hostname> <port>
if (argc != 3)
{
printf("usage: %s <hostname> <port>\n", argv[0]);
// terminate program
exit(0);
}

// initialize variables used as function arguments and in later calculations
int sock = 0; // socket file decriptor
int n = 0; // counter for bytes recieved
int lost = 0; // counter for packages dropped
int portno; // port number
int cli_size; // address size of client
double rtt = 0.0; // round trip time (RTT) counter
double min = 100.0; // minimum RTT during 10 pings
double max = 0.0; // maximum RTT during 10 pings
double avg = 0.0; // average RTT during 10 pings
double avgDivider = 0.0; // counts recieved messages, used to calculate "avg"
char recvBuff[MAXLINE]; // arr of chars that store messaged recieved by cli
char* hostname; // points to hostname of the server
char *ping = "PING"; // points to "PING" and print "PING" on server-side
struct sockaddr_in serv_addr; // serv_addr - made up of the socket address and port no.
struct hostent* server; // stores IP address of host
struct timeval t1, t2; // tracks duration of pings
hostname = argv[1]; // holds server hostname e.g. cell06-cse

// gets valid hostname
if ((server = gethostbyname(hostname)) == NULL) {
printf("ERROR, no such host as %s\n", hostname);
exit(0);
}
// create socket fd
if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
{
printf("socket error\n");
exit(EXIT_FAILURE);
}
bzero((char*)&serv_addr, sizeof(serv_addr)); // create name with wildcards
serv_addr.sin_family = AF_INET; // set server family: IPx4
memcpy((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr, server->h_length); // set server data
i.e. address, size
portno = atoi(argv[2]); // convert port to int for manipulation
serv_addr.sin_port = htons(portno); // pass int portno as argument
cli_size = sizeof(serv_addr); // getter - get size of client
// for each 10 PING messages, this loop will execute
for (int i = 1; i <= 10; i++)
{

// get system time at start of message transmission
currentTime(&t1, NULL);
// send ping request message using UDP socket
n = sendto(sock, (const void*)ping, strlen(ping), MSG_CONFIRM, (const struct sockaddr*)&serv_addr,
sizeof(serv_addr));

// message was not recieved
if (n < 0)
{
printf("%2d: Sent... Timed Out\n", i);
lost++;
continue;
}
// client waits at least one second
fd_set set;
FD_ZERO(&set);
FD_SET(sock, &set);

struct timeval timeout;
timeout.tv_sec= 1; // set timeout to 1 second
timeout.tv_usec = 0;
// wait on socket to read
int sel = select(sock + 1, &set, NULL, NULL, &timeout);
// error with select funct
if (sel == -1)
{
printf("Select error\n");
exit(EXIT_FAILURE);
}
// timeout has reached one second
else if (sel == 0)
{
printf("%2d: Sent... Timed Out\n", i);
lost++;
continue;
}
// socket is successfully reading from client
else
{

// receieves messages

n = recvfrom(sock, recvBuff, sizeof(recvBuff), MSG_WAITALL, (struct sockaddr*)&serv_addr, &cli_size);
// if negative number of bytes are read
if (n < 0)

{
printf("Recvfrom error\n");
exit(EXIT_FAILURE);
}

// get system time at end of message transmission

currentTime(&t2, NULL);

// calculate rtt - time taken for message to be sent and recieved
// convert to milliseconds by multiplying by 1000

rtt = (double)(t2.tv_sec - t1.tv_sec) * 1000.0 + (double)(t2.tv_usec - t1.tv_usec) / 1000.0;

if (rtt < min) min = rtt; // re-assign min if rtt if less than initial min
if (rtt > max) max = rtt; // re-assign max if rtt if greater than initial max
avg += rtt; // sum up all round trips
avgDivider++; // increment msg recieved counter

// print each round trip time
printf("%2d: Sent... RTT=%.6f ms\n", i, rtt);
}
}
// calculate final average RTT
avg/=avgDivider;
// print all RTT and packets information specified:
printf("%d pkts xmited, %d pkts rcvd, %d%% pkt loss\n", 10, (10 - lost), (lost * 10));
printf("min: %.6f ms, max: %.6f ms, avg: %.6f ms \n", min, max, avg);
// close fd
close(sock);
return 0;
}
