/*
 ============================================================================
 Name        : wget.c
 Author      : Dekel
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>

#include "url_parser.h"
#define MESSAGE_LENGTH 8192

int main(int argc, char *argv[])
{
  int sock;
  struct sockaddr_in cli_name;

  //Check that the user inserted exactly command line argument.
  if (argc != 2)
  {
    perror("Usage:\n1. <protocol>://<hostname>/<path>\n2. <protocol>://<hostname>:<port>/<path>\n");
    exit(1);
  }

  printf("Resolving %s\n", argv[1]);

  //The system call creates a new socket(file descriptor). Every communication method starts with a socket.
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    {
	  perror ("Error opening channel\n");
      close(sock);
      exit(1);
    }

  struct timeval timeout;//Timeout for the connection
  timeout.tv_sec = 10;
  timeout.tv_usec = 0;
  //Set send timeout
  if (setsockopt (sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,
              sizeof(timeout)) < 0)
  {
	  perror ("Error setting socket timeout\n");
	  close(sock);
	  exit(1);
  }

  //Set receive timeout
  if (setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
              sizeof(timeout)) < 0)
  {
	  perror ("Error setting socket timeout\n");
	  close(sock);
	  exit(1);
  }

  url_t user_args;
  parse_url(argv[1], &user_args);//Send the user input argument.

  memset((char *)&cli_name,0, sizeof(cli_name));
  cli_name.sin_family = AF_INET;//Yes we are using IPV4
  cli_name.sin_addr.s_addr = user_args.ip.s_addr;//Copy the Internet address from result. address is already converted into network representation.
  cli_name.sin_port = htons(user_args.port);//We also need to convert the PORT to the correct endianess.(big or little - this is transparent to us)


  //This system call connects the socket to the server at IP 127.0.0.1 port 1337. Of course we uses IPV4
  if (connect(sock, (struct sockaddr *)&cli_name, sizeof(cli_name)) < 0)
    {
	  perror ("Error establishing communications\n");
      close(sock);
      exit(1);
    }

  char message[MESSAGE_LENGTH];
  snprintf(message, sizeof(message), "GET %s HTTP/1.0\r\nHost: %s\r\n\r\n", user_args.path, user_args.h_name);
  char data [MESSAGE_LENGTH + 1];//+1 for null terminator
  write(sock, message, sizeof(message));

  ssize_t length = 0;
  while((length = read(sock, data, sizeof(data) - 1)) > 0)
  {
	  data[length] = '\0';
	  printf("%s", data);
  }
  printf("\nExiting now.\n");

  //This system call closes the file descriptor of our connection.
  close(sock);
  exit(1);
}
