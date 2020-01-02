
#include <sys/types.h>
#include <unistd.h>          
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>    
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#define SIM_LENGTH 10 


typedef struct url {
    char* protocol;
    char* hostname;
    unsigned short port;
    char* path;
} url;

struct sockaddr_in resolve_address(char* hostname)
{
  struct addrinfo* res;
  //char* hostaddr;
  struct sockaddr_in* saddr;
  
  
    if (0 != getaddrinfo(hostname, NULL, NULL, &res)) {
      fprintf(stderr, "Error in resolving hostname %s\n", hostname);
      exit(1); // temp
    }

    // addrinfo.ai_addr can be casted into sockaddr or sockaddr_in
  saddr = (struct sockaddr_in*)res->ai_addr;
  //coverting between endians.
 // hostaddr = inet_ntoa(saddr->sin_addr);
 return *saddr;
}

// PARSING <protocol>://<hostname>:<port>/<path>
// or
// PARSING <protocol>://<hostname>/<path>
struct url parse_url(char* source) 
{
    url u;
    int len = strlen(source);
    int i   = 0; // index on source
    int k   = 0; // index on HostName or path inside struct of url
    unsigned short port= 0; // decimal value of parsed string for "port"
    char c  = *source;
    
    // parse PROTOCOL
    u.protocol = (char*) malloc(len*sizeof(char));
    while(c != ':' && i < len)
    {
        *(u.protocol+i) = c;
        c = *(source+(++i));
    }
    *(u.protocol+i) = '\0';
    
    i+=3; //advance TODO: validate existance of "//""
    c = *(source+(i));
    
    // parse HOSTNAME
    u.hostname = (char*) malloc(len*sizeof(char));
    while(c != ':' && c != '/' && i < len)
    {
        *(u.hostname+(k++)) = c;
        c = *(source+(++i));
    }
    *(u.hostname+k) = '\0';
    
    // check type
    // if port
    if(c == ':') // port mode
    {
        c = *(source+(++i));
        while(c != '/' && i < len)
        {
		    port = port * 10 + ( c - '0' );
            c = *(source+(++i));
        }
        u.port = port;
    } else u.port = 80;
    
    
    k = 0; // reset pos
    
    u.path = (char*) malloc(len*sizeof(char));
    c = *(source+(++i));
    while(c != '/' && i < len)
    {
        *(u.path+(k++)) = c;
        c = *(source+(++i));
    }
   *(u.path+k) = '\0';
   if(strlen(u.path) == 0)
        (u.path) = "/\0";
   
    return u;
}


int main(int argc, char* argv[])
{ 
  int sock; 
  struct sockaddr_in cli_name, dest_address; 
  int count;
  int value; 

    // if there is no host provided by the user in command line then exit.
  if (argc != 2) {
    perror("Usage: net_client <hostname>\n");
    exit(1);
  }

  // get hostname string from command line argument
  char* hostname = argv[1];
  // parse command line.
  url u = parse_url(hostname);
  printf("%s\n",hostname);
  printf("Protocol: %s, Hostname: %s, Port: %hu, Path: %s\n", u.protocol, u.hostname, u.port, u.path);


  // get IP address from HOST address
  dest_address = resolve_address(u.hostname);
  printf("%s\n",inet_ntoa(dest_address.sin_addr));
  hostname = inet_ntoa(dest_address.sin_addr);

  printf("Client is alive and establishing socket connection.\n");
  
  // create new TCP socket.
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    { perror ("Error opening channel");
      close(sock);
      exit(1);
    }
      
  // create a destination configuration with given port and host parsed by the parse_url function
  bzero(&cli_name, sizeof(cli_name)); 
  cli_name.sin_family = AF_INET; 
  cli_name.sin_addr.s_addr = inet_addr(hostname); 
  cli_name.sin_port = htons(u.port);


  if (connect(sock, (struct sockaddr *)&cli_name, sizeof(cli_name)) < 0)
    { perror ("Error establishing communications");
      close(sock);
      exit(1);
    }

    /* construct an HTTP request: 
    GET /PATH HTTP/1.0
    Host: HOSTNAME
    */
   char* ch = (char*) malloc(sizeof(char)*80);
   memset(ch, '\0', 80);
   ch = strcat(ch,"GET ");
   ch = strcat(ch,u.path);
   ch = strcat(ch," HTTP/1.0\r\nHost: ");
   ch = strcat(ch,u.hostname);
   ch = strcat(ch,"\r\n\r\n");

   printf("%s\nSize of request: %zu\n", ch, strlen(ch));
   char* response = (char*) malloc(sizeof(char)*65550);
   send(sock, ch, strlen(ch),0);
    int i =0;
    // read incoming data.
    while(read(sock, (response+(i++)), 1)>0);

  printf("Client has received %s from socket.\n", response);
  printf("Exiting now.\n");

  close(sock); 
  exit(0); 

} 
