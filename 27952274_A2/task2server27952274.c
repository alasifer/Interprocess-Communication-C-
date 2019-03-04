#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>


#define SOCKETNAME "/tmp/a2-27952274"

int main(int argc, char *argv[])
{

    char buffer[1024]; /*buffer to store the data to be recieve from the client*/
    int n, sock,nsock, len; /* n= to store the exact amount of byte to be received from the client*/
                            /* sock= to store the descriptor of the server*/
                            /* len= to store the lenght of the struct sockaddr_un*/
                            /* nsock = to store the descriptor of the client socket */
                                
	char input[50],termi[10]; /* input= buffer to store the exact data recevied from the client */
                              /* termi= buffer to store data received from the TTY */
    struct sockaddr_un name;  /* a struct that consist of the path of the socket and type */
    fd_set fds; /* a var to store all descriptor needed to be passed to the select() */

    printf("<button {</#x1F5D9>} border-color={red} { Quit} color={black} background-color={red} spacing-left = 92,{%} onclick=<putln {Q}>>");

    if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
        perror("server: socket");
        exit(1);    
    }
    memset(&name, 0, sizeof(struct sockaddr_un)); /* to empty the struct name */
    name.sun_family = AF_UNIX;
	
    strcpy(name.sun_path, SOCKETNAME);
	
    len = sizeof(name.sun_family) + strlen(name.sun_path);

    unlink(name.sun_path); /* if there is any previouse connection with the same path, this statement will terminate that connection */

    if (bind(sock, (struct sockaddr*) &name, SUN_LEN(&name)) < 0){
        perror("server: bind"); 
        exit(1);
    }
    if (listen(sock, 10) < 0){
        perror("server: listen");
        exit(1);    
    }

    if ((nsock = accept(sock, (struct sockaddr *) &name, &len)) < 0){
        perror("server: accept");
        exit(1);
    }
    
	n = recv(nsock, buffer, sizeof(buffer),0); /*to recieve the name of the socket */

    strncpy(input, buffer, n);

    printf("<b:box color={black} {<box background-color={blue} width=100,{%} color={white} {%s1}>} width=100,{%} border-color={blue} >",input);
    
    while (n > 0){
        FD_ZERO(&fds);
        FD_SET(nsock, &fds); /* add the descriptor of the client connection */
        FD_SET(0, &fds);     /* add the descriptor of reading from the terminal */

        select(FD_SETSIZE, &fds, NULL,NULL, NULL);

        if (FD_ISSET(nsock,&fds)){
            n = recv(nsock, buffer, sizeof(buffer),0);
            memset(input, 0, sizeof(input)); /* to empty the input var */
            strncpy(input, buffer, n);
            memset(buffer, 0, sizeof(buffer)); /* to empty the buffer */
            printf("<b.push {</n> %s} color={white}>",input);            
        }
            
        else if (FD_ISSET(0, &fds)){
            scanf("%[^\n]%*c", termi);
            if (strcmp(termi, ("Q")) == 0){
                close(nsock);
                close(sock);
                exit(0);
            }
        }       
    }
    close(nsock);
    close(sock);
    exit(0);
}
