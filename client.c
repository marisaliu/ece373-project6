/*
 * echoclient.c - An echo client
 */
/* $begin echoclientmain */
#include "csapp.h"
char buf[101];
char buf2[101];
int quit;
rio_t rio;

void *thread(void *vargp){
	//int connfd = *((int *)vargp);
	Pthread_detach(pthread_self());
	//Free(vargp);
	//Close(connfd);*/
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
//    rio_t rio;
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
//		for(int i=0; i<101; i++) buf[i] = '\0';
		host = argv[1];
    port = argv[2];
		strcpy(name, "@");

    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);
		//send username to server
		strcat(name, argv[3]);
		Rio_writen(clientfd, name, 21);//strlen(argv[3]));
    //printf("sent\n");
		if(pthread_create(&tid, NULL, thread, NULL) != 0){
			printf("error\n");
			exit(0);
		}
		//take input
		while (!quit) {
			printf("> ");
			Fgets(buf, 101, stdin);
//			printf("buf: %s\n",buf);
			if(strlen(buf) > 100){
				printf("error: message too long\n");
			}
			else{
//				printf("buf: %s\n", buf);
				Rio_writen(clientfd, buf, strlen(buf));
				if(strcmp(buf, "quit\n")==0){
					quit = 1;
					break;
				}
			}
		//	Rio_readlineb(&rio, buf, MAXLINE);
		//	if(strcmp(buf, "0")){
		//		printf("error: no user with that name\n");
		//	}
		//	Fputs(buf, stdout);
    }
		quit =1;
		printf("GoodBye!\n");
    Close(clientfd); //line:netp:echoclient:close
    exit(0);
}
/* $end echoclientmain */
