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

int parseUser(char * in){
  char *newin = (char *)malloc(21*sizeof(char));;
	strcpy(newin, in);
	char *out = strtok(newin," ");
//	printf("out: %s\n", out);
	int i;
	for(i = 0; i<51; i++){
		if(i == 50){
			return 51;
			break;
		}
//		printf("userArr: %d, %s\n", i, userArr[i]);
		if(strcmp(out, userArr[i]) == 0) return i;
	}
	return i;
}

void chat(int connfd)
{
	int index = 0;
  char buf[101];
	char name[21];
	char *message = (char *)malloc(101*sizeof(char));
	int userIndex;
  rio_t rio;
	for(int i = 0; i < 101; i++) buf[i] = ' ';
	Rio_readinitb(&rio, connfd);
  Rio_readlineb(&rio, name, 21);
	printf("%s joined \n",name);
	if(strlen(buf) != 0){
	while((userArr[index] != NULL) && (strcmp(userArr[index], " ") != 0)){
		index++;
	}
	strcpy(userArr[index], name); 
	ipArr[index] = connfd;
//	printf("userArr now: %s\n", userArr[index]);
//	for(int j = 0; j < 50; j++) printf("user%s\n", userArr[j]);
//	char* bufHead = buf;
	while(1){
		memset(buf, 0, sizeof(buf));		
		Rio_readlineb(&rio, buf, 101);
		//printf("read: %s\n", buf);
		if(strlen(buf) > 1){
	//	printf("buf11: %s\n", buf + 1);
		if(strcmp(buf +1, "quit") == 0) break;
		else if(strcmp(buf, "list-users") == 0){
			for(int j = 0; j < 50; j++){
				if(userArr[j] != NULL) rio_writen(connfd, userArr[j], 101);
			}
		}
		else{
			userIndex = parseUser(buf + 1);
		//	printf("buf2: %s\n", buf);
			if(userIndex == 51){
				strcpy(buf, "ERROR! NO USER OF THAT NAME!	Please enter a valid user");
				rio_writen(connfd, buf, 101);
			}
			else{	
				strcpy(message, (buf+1+ strlen(userArr[userIndex])));
			//	printf("message1: %s\n", message);
				char temp[21];
				strcpy(temp, userArr[index]);
				message = strcat(temp, message);
			//	printf("message: %s\n", message);
				rio_writen(ipArr[userIndex], message, 101);
			}
		}
		}
	}
}
	printf("%s left \n", userArr[index]);
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
		//	printf("tid: %d\n", tid);
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
