
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <getopt.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>


//#define BUFSIZE 1219
#define BUFSIZE 16

#define USAGE                                                                 \
"usage:\n"                                                                    \
"  echoserver [options]\n"                                                    \
"options:\n"                                                                  \
"  -p                  Port (Default: 19121)\n"                                \
"  -m                  Maximum pending connections (default: 1)\n"            \
"  -h                  Show this help message\n"                              \

/* OPTIONS DESCRIPTOR ====================================================== */
static struct option gLongOptions[] = {
  {"port",          required_argument,      NULL,           'p'},
  {"maxnpending",   required_argument,      NULL,           'm'},
  {"help",          no_argument,            NULL,           'h'},
  {NULL,            0,                      NULL,             0}
};


int main(int argc, char **argv) {
  int option_char;
  int portno = 19121; /* port to listen on */
  int maxnpending = 1;

  // Parse and set command line arguments
  while ((option_char = getopt_long(argc, argv, "p:m:hx", gLongOptions, NULL)) != -1) {
   switch (option_char) {
      case 'p': // listen-port
        portno = atoi(optarg);
        break;
      default:
        fprintf(stderr, "%s ", USAGE);
        exit(1);
      case 'm': // server
        maxnpending = atoi(optarg);
        break;
      case 'h': // help
        fprintf(stdout, "%s ", USAGE);
        exit(0);
        break;
    }
  }

    setbuf(stdout, NULL); // disable buffering

    if ((portno < 1025) || (portno > 65535)) {
        fprintf(stderr, "%s @ %d: invalid port number (%d)\n", __FILE__, __LINE__, portno);
        exit(1);
    }
    if (maxnpending < 1) {
        fprintf(stderr, "%s @ %d: invalid pending count (%d)\n", __FILE__, __LINE__, maxnpending);
        exit(1);
    }


  /* Socket Code Here */
/*
 steps to datatransfer.
 1. create a socket
 2. bind it with an address
 3. listen for requests
 4. connect to the client

 */
    struct addrinfo ServerAddressHint, *ServerAddress;
    memset(&ServerAddressHint, 0, sizeof(ServerAddressHint));
    
    ServerAddressHint.ai_family = AF_UNSPEC;
    ServerAddressHint.ai_socktype = SOCK_STREAM;
    ServerAddressHint.ai_flags = AI_PASSIVE;

//    char* portNoChar = itoa(portno);

    char portNoChar[20];
    snprintf(portNoChar, 20, "%d",portno);

    int serverAddressGetStatus = getaddrinfo(NULL, portNoChar, &ServerAddressHint, &ServerAddress);

    if (serverAddressGetStatus != 0)
    {
        fprintf(stderr,"couldn't get address\n");
        exit(1);
    }
    
    int serverSocket = socket(ServerAddress->ai_family, ServerAddress->ai_socktype, 0);

    int socketOptionValue = 1;
    int settingSocketOptionStatus = setsockopt(serverSocket,SOL_SOCKET,SO_REUSEADDR, &socketOptionValue,sizeof(int));
    if (settingSocketOptionStatus!=0)
    {
        fprintf(stderr,"could not set socket options");
        exit(1);
    }
    
    int bindStatus = bind(serverSocket, ServerAddress->ai_addr, ServerAddress->ai_addrlen);
    if(bindStatus!=0){
        fprintf(stderr, "couldn't bind socket\n");
          exit(1);
    }

    int listenStatus = listen(serverSocket, maxnpending);

    if(listenStatus!=0){
        fprintf(stderr, "couldn't listen onto the socket\n");
          exit(1);
    }

    //freeaddrinfo(ServerAddress);
    
    
    while(1){
    
    int newClientSocket = accept(serverSocket, ServerAddress->ai_addr, &ServerAddress->ai_addrlen);

//    int newClientSocket = connect(serverSocket, ServerAddress->ai_addr, ServerAddress->ai_addrlen);

    char buf[BUFSIZE];
        memset(buf,0,sizeof(buf));
    ssize_t receivedBytes = recv(newClientSocket,buf,BUFSIZE-1,0);

//        printf("received from client to server %ld\n",receivedBytes);
        
    buf[receivedBytes] = '\0';
    printf("%s",buf);
    
        send(newClientSocket,buf,BUFSIZE-1,0);
        
    close(newClientSocket);
    }

    return 0;

}

