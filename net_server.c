#include <sys/types.h>
#include <unistd.h>          
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>     
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#define SIM_LENGTH 10 
#define PORT 1337 

void clean_up(int cond, int *sock)
{ printf("Exiting now.\n");
  close(*sock); 
  exit(cond);
} 

int main(void)
{ 
  int sock; 
  int connect_sock; 
  struct sockaddr_in serv_name; 
  socklen_t len; 
  int count;
  
 // to create a new socket IPv4 TCP
  sock = socket(AF_INET, SOCK_STREAM, 0);  
  if (sock < 0)
    { perror ("Error opening channel");
      clean_up(1, &sock);
    }
  
  memset((char *)&serv_name,0, sizeof(serv_name)); 
  serv_name.sin_family = AF_INET;                                   
  serv_name.sin_port = htons(PORT); 

	// socket connection to an address port
  if (bind(sock, (struct sockaddr *)&serv_name, sizeof(serv_name)) < 0)
    { perror ("Error naming channel");
      clean_up(1, &sock);
    }
      
  printf("Server is alive and waiting for socket connection from client.\n");
  listen(sock, 1);  // to be ready to accept new connection

  len = sizeof(serv_name);
  connect_sock = accept(sock, (struct sockaddr *)&serv_name, &len);// this function listen to TCP-syn .once the server get the syn, a 3 way handshake.

  for (count = 1; count <= SIM_LENGTH; count++)
    { 
	  write(connect_sock, &count, 4);// write data to the socket
      printf("Server has written %d to socket.\n", count);
    }

  close(connect_sock); 
  clean_up(0, &sock); 

}
