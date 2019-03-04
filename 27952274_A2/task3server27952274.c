#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdbool.h>

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
    int l[] = {-1,-1,-1,-1,-1}; /* initialise an array with same number of maximum number of client allowed */
    int max_socket = 5; /*maximum number of socket allowed */
    int numOfclient = 0;
    struct sockaddr_un name;/* a struct that consist of the path of the socket and type */
    fd_set fds;/* a var to store all descriptor needed to be passed to the select() */
    
    
    printf("<button {</#x1F5D9>} border-color={red} { Quit} color={black} background-color={red} spacing-left = 92,{%} onclick=<putln {Q}>>");

    if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
        perror("server: socket");
        exit(1);    
    }
    memset(&name, 0, sizeof(struct sockaddr_un));/* to empty the struct name */
    name.sun_family = AF_UNIX;
	
    strcpy(name.sun_path, SOCKETNAME);
	
    len = sizeof(name.sun_family) + strlen(name.sun_path);

    unlink(name.sun_path);/* if there is any previouse connection with the same path, this statement will terminate that connection */

    if (bind(sock, (struct sockaddr*) &name, SUN_LEN(&name)) < 0){
        perror("server: bind"); 
        exit(1);
    }
    if (listen(sock, max_socket) < 0){
        perror("server: listen");
        exit(1);    
    }


    while (1){
        FD_ZERO(&fds);
        FD_SET(0, &fds);/* add the descriptor of reading from the terminal */
        FD_SET(sock, &fds);/* add the descriptor of the server */

        for (int i=0; i<max_socket; i++){
            if (l[i] != -1){
                FD_SET(l[i], &fds); /* to store the file descriptor of each connected client */
            }
        }

        select(FD_SETSIZE, &fds, NULL,NULL, NULL);
        
        if (FD_ISSET(sock, &fds)){ /* to check the if the file descriptor called is the server */
            if (numOfclient < 5){ /*toc check if maximum number of clients connection reached */
                            
                if ((nsock = accept(sock, (struct sockaddr *) &name, &len)) < 0){
                    perror("server: accept");
                    exit(1);
                }
                /* add the new client connection descriptor to the list */
                for (int i=0; i < max_socket; i++){
                    if (l[i] == -1){
                        l[i] = nsock;
                        FD_SET(l[i], &fds);
                        n = recv(l[i], buffer, sizeof(buffer),0);
                        strncpy(input, buffer, n);
                        printf("<client%d:box color={black} {<box background-color={blue} width=100,{%} color={white} {%s%d}>} width=100,{%} border-color={blue} >",i,input,i+1);
                        numOfclient ++;
                        break;
                    }
                }
            } 
        }        
        for (int i=0; i<max_socket; i++){ /* iterate over the list of connected client descriptor */
            if (FD_ISSET(l[i],&fds)){
                n = recv(l[i], buffer, sizeof(buffer),0);
                if (n == 0){/*check if the client terminated */
                    close(l[i]);
                    l[i] = -1;
                    break;
                }
                memset(input, 0, sizeof(input));/* to empty the input var */
                strncpy(input, buffer, n);
                memset(buffer, 0, sizeof(buffer));/* to empty the buffer */
                printf("<client%d.push {</n> %s} color={white}>",i,input);            
            }
           
        }
   
        if (FD_ISSET(0, &fds)){ /* to check if the input is from the TTY */
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
