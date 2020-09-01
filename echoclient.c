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

/* Be prepared accept a response of this length */
//#define BUFSIZE 1219
#define BUFSIZE 16

#define USAGE                                                                       \
    "usage:\n"                                                                      \
    "  echoclient [options]\n"                                                      \
    "options:\n"                                                                    \
    "  -s                  Server (Default: localhost)\n"                           \
    "  -p                  Port (Default: 19121)\n"                                  \
    "  -m                  Message to send to server (Default: \"Hello world.\")\n" \
    "  -h                  Show this help message\n"

/* OPTIONS DESCRIPTOR ====================================================== */
static struct option gLongOptions[] = {
    {"server", required_argument, NULL, 's'},
    {"port", required_argument, NULL, 'p'},
    {"message", required_argument, NULL, 'm'},
    {"help", no_argument, NULL, 'h'},
    {NULL, 0, NULL, 0}};

/* Main ========================================================= */
int main(int argc, char **argv)
{
    int option_char = 0;
    char *hostname = "localhost";
    unsigned short portno = 19121;
    char *message = "Hello World!!";

    // Parse and set command line arguments
    while ((option_char = getopt_long(argc, argv, "s:p:m:hx", gLongOptions, NULL)) != -1)
    {
        switch (option_char)
        {
        case 's': // server
            hostname = optarg;
            break;
        case 'p': // listen-port
            portno = atoi(optarg);
            break;
        default:
            fprintf(stderr, "%s", USAGE);
            exit(1);
        case 'm': // message
            message = optarg;
            break;
        case 'h': // help
            fprintf(stdout, "%s", USAGE);
            exit(0);
            break;
        }
    }

    setbuf(stdout, NULL); // disable buffering

    if ((portno < 1025) || (portno > 65535))
    {
        fprintf(stderr, "%s @ %d: invalid port number (%d)\n", __FILE__, __LINE__, portno);
        exit(1);
    }

    if (NULL == message)
    {
        fprintf(stderr, "%s @ %d: invalid message\n", __FILE__, __LINE__);
        exit(1);
    }

    if (NULL == hostname)
    {
        fprintf(stderr, "%s @ %d: invalid host name\n", __FILE__, __LINE__);
        exit(1);
    }

    /* Socket Code Here */
    /*
     Client thread
     create socket
     connect to the address
     send message using recv and send
     */

    // Creating a socket
    
    
    struct addrinfo addHint, *serverAddress;
    
    memset(&addHint, 0, sizeof(addHint));
    
    addHint.ai_family = AF_UNSPEC;
    addHint.ai_flags = AI_PASSIVE;
    addHint.ai_socktype = SOCK_STREAM;
    
    char portNoChar[20];
    snprintf(portNoChar,20,"%d",portno);
    
//    itoa(portno,portNoChar,10);
    
    int gettingAddrInfo = getaddrinfo(hostname, portNoChar, &addHint, &serverAddress);
    if(gettingAddrInfo!=0){
        fprintf(stderr, "couldn't get address info\n");
    }
    
    int clientSocket = socket(serverAddress->ai_family, serverAddress->ai_socktype, 0);
    
    
    
    int connectionToServer = connect(clientSocket, serverAddress->ai_addr, serverAddress->ai_addrlen);
    
    if(connectionToServer!=0){
        fprintf(stderr, "couldn't connect to the specified server\n");
    }
    
    char buffer[BUFSIZE];
    memset(buffer,0,sizeof(buffer));
//    ssize_t sentLength =
    send(clientSocket, message, BUFSIZE-1, 0);
    
    ssize_t receivedLength = recv(clientSocket,buffer,BUFSIZE-1,0);
//    printf("received at client from server %ld\n",receivedLength);
    buffer[receivedLength] = '\0';
    printf("%s",buffer);

    freeaddrinfo(serverAddress);
    close(clientSocket);

    return 0;
}
 
