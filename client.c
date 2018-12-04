/*
 * echoclient.c - An echo client
 */
/* $begin echoclientmain */
#include "csapp.h"

int main(int argc, char **argv) 
{
    int clientfd;
    char *host, *port, buf[MAXLINE];
    rio_t rio;
		
    if (argc != 4) {
			fprintf(stderr, "usage: %s <host> <port> <username>\n", argv[0]);
			exit(0);
    }
		if(strlen(argv[3]) > 20){
			printf("username should not exceed 20 characters\n");
			exit(0);
		}
    host = argv[1];
    port = argv[2];

    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);
		//send username to server
		Rio_writen(clientfd, argv[3], strlen(argv[3]));
    //take input
		while (1) {
			printf("> ");
			Fgets(buf, MAXLINE, stdin);
			Rio_writen(clientfd, buf, strlen(buf));
			Rio_readlineb(&rio, buf, MAXLINE);
			if(strcmp(buf, "0")){
				printf("error: no user with that name\n");
			}
			Fputs(buf, stdout);
    }
		printf("GoodBye!\n");
    Close(clientfd); //line:netp:echoclient:close
    exit(0);
}
/* $end echoclientmain */
