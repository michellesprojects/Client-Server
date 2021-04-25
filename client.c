
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define MAX 258 //the maximum size of a message
#define SOCKETADDRESS struct socket_address

static int socketio; //the file I/O for the socket
static char buffer[MAX]; //where the message will be stored

//function used to close to connection if there is an error
void error(char *err_message){
    printf("ERROR: %s\n",err_message); //prints error message
    memset(&buffer,0,sizeof(buffer)); //clears the buffer
close(socketio); //closes the socket
    exit(1);
}

//function that makes the Query Message
void messageMake(int socketio)
{
    
    int i; //coutner for buffer index


        memset(&buffer,0,sizeof(buffer)); //sets buff mem to 0 so there is no garbage in the emssage
    
        printf("ENTER MESSAGE: "); //prompts user to enter client query
    
        i = 2;


    //check to see if first value is Q
        char  type = getchar();
        if(type != 'Q')
            error("FORMAT INCORRECT");

    //gets length
        int length = 0;
        scanf("%d",&length);

    //error checks length
        if(length == 0)
            error("LENGTH TOO SMALL");
        buffer[0] = 'Q';
        buffer[1] = length;

    //find the size of the string and set the values of the buffer
        int char_count = 0;
        while ((buffer[i] = getchar()) != '\n'){
            char_count++;
            i+=1;

        }
    
   
        buffer[1] = char_count-'0';
    
    //error check message
       if(char_count >= 255)
        error("MESSAGE SIZE TOO LARGE");

        if(length != char_count){
            error("STRING LENGTH MISMATCH");
        }


        if(buffer != 0)
        write(socketio, buffer, sizeof(buffer));

         memset(&buffer,0,sizeof(buffer));
         if(buffer != 0)
        read(socketio, buffer, sizeof(buffer));
    
        printf("RESPONSE RECIEVED: %s", buffer);

        close(socketio);

}

int main()
{
    
    

    struct sockaddr_in server_ip;

    // create socket
    socketio = socket(AF_INET, SOCK_STREAM, 0);
    
    //error checking for socket creation failure
    if (socketio <0) {
        error("CANNOT CREATE CLIENT SOCKET");
    }


    memset(&server_ip, 0, sizeof(server_ip));
    
    server_ip.sin_family = AF_INET;
    server_ip.sin_addr.s_addr = inet_addr("127.0.0.1"); //assign IP
    server_ip.sin_port = htons(8000); //assign port


    if (connect(socketio, (SOCKETADDRESS*)&server_ip, sizeof(server_ip)) != 0) {
        error("FAILED ESTABLISH CONNECTION");
    }
    else
        printf("TCP CONNECTION ESTABLISHED\n");


    messageMake(socketio);

}
