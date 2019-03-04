/*
 * Ahmed Abdulwahid Omar Alasaifer
 * 27952274
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SOCKETNAME "/tmp/a2-27952274"

int main(int argc, char *argv[])
{

    char buffer[1024];/*buffer to store the data to be sendt to the server*/
    int n, sock, len; /* n= to store the exact amount of byte to be sent to the server*/
                      /* sock= to store the descriptor of the socket*/
                      /* len= to store the lenght of the struct sockaddr_un*/
    
    struct sockaddr_un name; /* a struct that consist of the path of the socket and type */

    if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
        perror("client: socket");
        exit(1);    
    }
    memset(&name, 0, sizeof(struct sockaddr_un)); /* to empty the struct name */
    name.sun_family = AF_UNIX;
	
    strcpy(name.sun_path, SOCKETNAME);
    len = sizeof(name.sun_family) + strlen(name.sun_path);

    shutdown(sock, 0); /* to prevent the socket from receiving data */

    if (connect(sock, (struct sockaddr *) &name, SUN_LEN(&name)) < 0){
        perror("client: connect");
        exit(1);
    }
    
    
    char Name[] = "client\n";
    if (send(sock, Name, sizeof(Name),0) < 0){
        perror("client: send");
        exit(1);
    }
	
	n = 5; /*to give the n a value greater than 0, so that it starts the loop */
	
	while ((n = read(0, buffer, n)) > 0){
        send(sock, buffer, n,0);
    }
    close(sock);
    exit(0);
}
