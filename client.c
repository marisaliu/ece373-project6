/*
 * echoclient.c - An echo client
 */
/* $begin echoclientmain */
#include "csapp.h"
char buf[101];
char buf2[101];
int quit;
rio_t rio;

//receive messages
void *thread(void *vargp){
	Pthread_detach(pthread_self());
	while(!quit){
		Rio_readlineb(&rio, buf2, 101);
		if((strlen(buf2) > 1))	Fputs(buf2, stdout);
	}
	return NULL;
}

int main(int argc, char **argv) 
{
    int clientfd;
    char *host, *port;
		char *name = (char *)malloc(strlen(argv[3])*sizeof(char));
		pthread_t tid;	
		quit = 0;
		
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
		strcpy(name, "@");

    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);
		//send username to server
		strcat(name, argv[3]);
		Rio_writen(clientfd, name, 21);
		if(pthread_create(&tid, NULL, thread, NULL) != 0){
			printf("error\n");
			exit(0);
		}
		//take input
		while (!quit) {
			printf("> ");
			Fgets(buf, 101, stdin);
			if(strlen(buf) > 99){
				printf("error: message too long\n");
			}
			else{
				Rio_writen(clientfd, buf, strlen(buf));
				if(strcmp(buf, "quit\n")==0){
					quit = 1;
					break;
				}
			}
    }
		quit =1;
		printf("GoodBye!\n");
    Close(clientfd); 
    exit(0);
}
/* $end echoclientmain */
