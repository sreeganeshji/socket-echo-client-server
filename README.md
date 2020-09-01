# socket-echo-client-server
Establishing connection between client and a server with sockets in C.

usage:
```
./echoclient
./echoserver
```

# Project Design
In this project we have a simple server and client interface using sockets. The client provides a message to the server and the server sends that message back to the client. 

<image src="https://raw.githubusercontent.com/sreeganeshji/socket-echo-client-server/master/Illustrations/echo.png" height=150>
 
# Trade offs considered 
Since the message exchanged between the client and server is small in length, a cumulative buffer implementation was not considered which would allow for transfer of larger files. 

# Flow of control 
The server takes the listening port number as the input and creates a socket which is bound to the obtained server address. The server listens for any connection requests to the bound address. 
The client takes the requesting port number, the server address and the message as inputs and creates a socket connecting to this address. Once connected, the client sends a message to the server, and the server responds by sending the message back to the client.

# Code implementation 
The server creates a listening socket and waits in a while loop for client requests. The client creates a socket and connects to the server. The server then receives the message and sends back to the client. The client prints the message and exits from execution after destroying the allocated memory. 

# Code testing 
The code was tested to work for different port numbers, messages, invalid port numbers, and so on. 
References: the main reference that was used for this implementation was Beej’s Guide to network programming pg 29-30. 
