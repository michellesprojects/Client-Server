#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#define MAX 258
#define SOCKETADDRESS struct socket_address
static int socketio;
static char buffer[MAX];
void error(char *err_message){
    printf("ERROR: %s\n",err_message);
    close(socketio);
        exit(1);
}

// Function designed for chat between client and server.
void addressBook(int socketio)
{
 
        bzero(buffer, MAX);

        // read the message from client and copy it in buffer
        read(socketio, buffer, sizeof(buffer));


//reading buffer to see if message format is correct
if(buffer[0] == 0){
    error("CANNOT READ QUERY");
}

        // print buffer which contains the client contents
        if(buffer != 0)
        printf("QUERY RECIEVED: %s\t",buffer);


//set up the server response
    buffer[0] = 'R'; //set first byte

    if(strstr(buffer,"@gmail.com")){
        char name [] = "Email Wizard";
       // buff[1] = ((char) sizeof(name));
        buffer[1] = 17 - '0';
        memcpy(buffer+2, name,sizeof(name));
    }

else if(strstr(buffer, "@aol.com")){
    char name [] = "OldAgent";
    buffer[1] = 20 - '0';
    memcpy(buffer+2, name,sizeof(name));
}
else{
    char name [] = "NOT FOUND";
    buffer[1] =  '9';
    memcpy(buffer+2, name,sizeof(name));
}

        // and send that buffer to client
    if(buffer != 0){
        write(socketio, buffer, sizeof(buffer));
    }
       close(socketio);


    }

int main()
{
    int servercont, size_of_server;
    struct sockaddr_in servaddress, blank_size;

    // socket creation
    socketio = socket(AF_INET, SOCK_STREAM, 0);

//socket vertification
    if (socketio == -1) {
        error("FAILED TO CREATE SOCKET");
    }


//clearing garbage in memory to be able to store the address of the server
memset(&servaddress,0,sizeof(servaddress));



    servaddress.sin_family = AF_INET; //assigning the address family
    servaddress.sin_addr.s_addr = htonl(INADDR_ANY);  //attempting to bind the socket to all local interfaces
    servaddress.sin_port = htons(8000); //port

    // socket now has assigned values, time to bind it......
    if ((bind(socketio, (SOCKETADDRESS*)&servaddress, sizeof(servaddress))) != 0) {
        error("FAILED TO BIND SOCKET");
    }

    // listen on the socket for connections, 5 is queue size
    if ((listen(socketio, 5)) != 0) {
        error("LISTEN FAILED");

    }

    size_of_server = sizeof(blank_size);


//causes the process to block until a client connects to the server.
    //accepts the message
    servercont = accept(socketio, (SOCKETADDRESS*)&blank_size, &size_of_server);
    if (servercont < 0) {
        error("ACCEPT FAILED");
    }


    addressBook(servercont);

}
