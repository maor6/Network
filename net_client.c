

#include <sys/types.h>
#include <unistd.h>
#include <strings.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define SIM_LENGTH 10
#define IP_ADDRESS "127.0.0.1"
#define PORT 1337

int main(int argc, char *argv[])
{
  int sock;
  struct sockaddr_in cli_name;
  int count;
  int value;

  if (argc != 2) {// to check if the user inserted correct commend-line argument
    perror("Usage: hostnamelookup <hostname>\n");
    exit(1);
  }

  printf("Client is alive and establishing socket connection.\n");

  // to create a new socket IPv4 TCP
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    { perror ("Error opening channel");
      close(sock);
      exit(1);
    }

  char* hostname = argv[1];//get the user commend-line argument
  struct addrinfo* result;

  //this function retrieves the IP address of the hostname
  if (0 != getaddrinfo(hostname, NULL, NULL, &result)) {
    fprintf(stderr, "Error in resolving hostname %s\n", hostname);//Print to error standard(to the screen)
    exit(1);
  }

  struct sockaddr_in * host_addr = (struct sockaddr_in *)result->ai_addr;

  memset((char *)&cli_name,0, sizeof(cli_name));
  cli_name.sin_family = AF_INET;
  cli_name.sin_addr.s_addr = host_addr->sin_addr.s_addr;//copy the Internet address from result.
  cli_name.sin_port = htons(PORT);//convert the port to network.


  //connect the socket to the server
  if (connect(sock, (struct sockaddr *)&cli_name, sizeof(cli_name)) < 0)
    {
	  perror ("Error establishing communications");
      close(sock);
      exit(1);
    }

  for (count = 1; count <= SIM_LENGTH; count++)
    {
	 //reads data from the socket
	  read(sock, &value, 4);
      printf("Client has received %d from socket.\n", value);
    }

  printf("Exiting now.\n");


  close(sock);
  exit(0);

}