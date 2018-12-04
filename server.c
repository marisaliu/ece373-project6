/* 
 * echoservert.c - A concurrent echo server using threads
 */
/* $begin echoservertmain */
#include "csapp.h"
#include <string.h>
#include <stdio.h>
int maxClients = 0;
int clientIndex = 0;
void *thread(void *vargp);

void echo(int connfd)
{
  int n;
  char buf[MAXLINE];
	char* toUser;
  rio_t rio;
	Rio_readlineb(&rio, buf, MAXLINE)
	while(userArr[index] != NULL){
		index++;
	}
	userArr[index] = buf; 
	ipArr[index] = connfd;
  Rio_readinitb(&rio, connfd);
	while(1){
		Rio_readlineb(&rio, buf, MAXLINE)
		toUser = parseUser(buf)
		if(strcmp(toUser, ERROR)){
			strcpy(buf, "ERROR! NO USER OF THAT NAME!	Please enter a valid user");
			rio_writen(connfd, buf, strlen(buf));
		}
		else{
			int i = 0;
			char *message = buf + strlen(toUser);
			while(strcmp(userArr[i], toUser) != 0){
				i = (i+1)%50;
			}
			if(strcmp(message, "quit") == 0) break;
			if(strcmp(message, "list-users" == 0)){
				for(int j = 0; j < 50; j++){
					if(userArr[j] != NULL) rio_written(connfd, userArr[j], strlen(userArr[j]);
				}
			}
			rio_writen(ipArr[i], message, strlen(message));

		}


	}
//  while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
  //      printf("server received %d bytes\n", n);
   //     Rio_writen(connfd, buf, n);
    //}
}

int main(int argc, char **argv) 
{
   char userArr[50][21];   //creates an array of 50 users with max username length of 20 character
	 char ipArr[50][21];
	 int listenfd, *connfdp;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t tid; 

    if (argc != 2) {
	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(0);
    }
    listenfd = Open_listenfd(argv[1]);

    while (1) {
      clientlen=sizeof(struct sockaddr_storage);
			connfdp = Malloc(sizeof(int)); //line:conc:echoservert:beginmalloc
			*connfdp = Accept(listenfd, (SA *) &clientaddr, &clientlen); //line:conc:echoservert:endmalloc
			if((numClients +1) >= maxClients){
				printf("MAX CLIENTS REACHED \n");
				close(connfd);
			}

			Pthread_create(&tid, NULL, thread, connfdp);
    }
}

/* Thread routine */
void *thread(void *vargp) 
{  
    int connfd = *((int *)vargp);
    Pthread_detach(pthread_self()); //line:conc:echoservert:detach
    Free(vargp);                    //line:conc:echoservert:free
    echo(connfd);
    Close(connfd);
    return NULL;
}
/* $end echoservertmain */
