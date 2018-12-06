/* 
 * echoservert.c - A concurrent echo server using threads
 */
/* $begin echoservertmain */
#include "csapp.h"
#include <string.h>
#include <stdio.h>
int maxClients = 50;
int numClients = 0;
char userArr[50][21];   //creates an array of 50 users with max username length of 20 character
int ipArr[50];
void *thread(void *vargp);

char * parseUser(char * in){
  char *out = strtok(in," ") + 1;
	for(int i=0; i<51; i++){
		if(i == 50){
			strcpy(out, "ERROR");
			break;
		}
		if(strcmp(out, userArr[i])) break;
	}
	return out;
}

void chat(int connfd)
{
	int index = 0;
  char buf[101];
	char* toUser;
  rio_t rio;
	for(int i = 0; i < 101; i++) buf[i] = ' ';
	Rio_readinitb(&rio, connfd);
  Rio_readlineb(&rio, buf, 21);
	printf("Init User: %s \n", buf);
//	fflush(stdout);
	if(strlen(buf) != 0){
	while((userArr[index] != NULL) && (strcmp(userArr[index], " ") != 0)){
		index++;
	}
	strcpy(userArr[index], buf); 
	ipArr[index] = connfd;
	while(1){
		Rio_readlineb(&rio, buf, 101);
//		printf("buf: %s\n", buf);
		toUser = parseUser(buf);
		printf("@%s joined \n", toUser);
		if(strcmp(toUser,"ERROR")){
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
			else if(strcmp(message, "list-users") == 0){
				for(int j = 0; j < 50; j++){
					if(userArr[j] != NULL) rio_writen(connfd, userArr[j], strlen(userArr[j]));
				}
			}
			rio_writen(ipArr[i], message, strlen(message));
		}
		}
	}
	strcpy(userArr[index], " " );
	ipArr[index] = 0;
	numClients--;


//  while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
  //      printf("server received %d bytes\n", n);
   //     Rio_writen(connfd, buf, n);
  //  }//}
}

int main(int argc, char **argv) 
{
	 int listenfd, *connfdp;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t tid; 

    if (argc != 2) {
	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(0);
    }
    listenfd = Open_listenfd(argv[1]);
		for(int i=0; i<50; i++) strcpy(userArr[i], " ");
    while (1) {
      clientlen=sizeof(struct sockaddr_storage);
			connfdp = Malloc(sizeof(int)); //line:conc:echoservert:beginmalloc
			*connfdp = Accept(listenfd, (SA *) &clientaddr, &clientlen); //line:conc:echoservert:endmalloc
			if((numClients +1) >= maxClients){
				printf("MAX CLIENTS REACHED \n");
				int connfd = *((int *)connfdp);
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
    chat(connfd);
    Close(connfd);
    return NULL;
}
/* $end echoservertmain */
