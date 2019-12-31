#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
 
/*
void printsin(struct sockaddr_in *s, char *str1, char *str2) {
  printf("%s\n", str1);
  printf("%s: ", str2);
  -- port: sin->sin_port (host integer type) 
  -- IP: sin->sin_addr (IP in dotted-decimal notation) 
  printf("\n");
}
*/

 
int main(int argc, char *argv[])
{
  int socket_fd, cc, fsize;
  struct sockaddr_in  s_in, from;
  struct { char head; u_long  body; char tail;} msg;
  char m[256];


 
  socket_fd = socket (AF_INET, SOCK_DGRAM, 0);   // creat socket with IPv4 protocol UDP and 0 is default
 
  bzero((char *) &s_in, sizeof(s_in));  /* They say you must do this    */ //zero the socket s_in
 
  s_in.sin_family = (short)AF_INET; // host byte order (IPv4)
  s_in.sin_addr.s_addr = htonl(INADDR_ANY);    /* WILDCARD */   // host to network long
  s_in.sin_port = htons((u_short)0x3333); // host to network short
 
  //printsin( &s_in, "RECV_UDP", "Local socket is:"); 
  fflush(stdout); // clear stdout 
 
  bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in)); // connect address and the port with socket
 char g[] = "gofna";
  for(;;) { // to litsen all the time to reqeust
    fsize = sizeof(from); 
    cc = recvfrom(socket_fd,&m,sizeof(m),0,(struct sockaddr *)&from,&fsize); //from where we recived the socket
    //printsin( &from, "recv_udp: ", "Packet from:");
    printf("Got data ::%s\n",m);
    sendto(socket_fd,g,sizeof(g),0,(const struct sockaddr *)&from,sizeof(from));
    printf("msg has send");
    fflush(stdout); // clear stdout after we get the data that the client send
  }
  

  


   
  return 0;
}
