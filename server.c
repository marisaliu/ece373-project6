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
int n = 0;
	int index = 0;
  char buf[MAXLINE];
	char temp[MAXLINE];
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
	while(1){
		memset(buf, 0, sizeof(buf));	
		Rio_readlineb(&rio, buf, 101);
		printf("read: %s\n", buf);
		if(strlen(buf) > 1){
		printf("buf11: %s\n", buf);
		if(n>0){
			if(strcmp(buf, "quit\n") == 0) break;
			if(strcmp(buf, "list-users\n") == 0){
				for(int j = 0; j < 50; j++){
					if(strcmp(userArr[j], " ") != 0){
						strcpy(temp, userArr[j]);
						strcat(temp, "\n");
						rio_writen(connfd, temp, strlen(temp));
						}
				}
				continue;
			}
		}
		else if (n==0){
			if(strcmp(buf+1, "quit\n") == 0) break;
			else if(strcmp(buf+1, "list-users\n") == 0){
				for(int j = 0; j < 50; j++){
						if(strcmp(userArr[j], " ") != 0){
						strcpy(temp, userArr[j]);
						strcat(temp, "\n");
						rio_writen(connfd, temp, strlen(temp));}
				}
				continue;
			}
		}
		  if(n == 0){
				printf("N = 0\n");
				userIndex = parseUser(buf+1);
				n++;
				}
			else userIndex = parseUser(buf);
 	printf("userIndex: %d\n", userIndex);
 // 	printf("buf2: %s\n", buf);
			if(userIndex == 51){
			continue;
		//		printf("NO USER OF THAT NAME\n");
//			strcpy(buf, "ERROR! NO USER OF THAT NAME!	Please enter a valid user");
	//			rio_writen(connfd, buf, strlen(buf));
			}
					else{
				printf("userIndex: %d\n", userIndex);
				strcpy(message, (buf+1+ strlen(userArr[userIndex])));
				printf("message1: %s\n", message);
//				char temp[MAXLINE];
				strcpy(temp, userArr[index]);
				if(n==1) n++;
				else strcat(temp, " ");
				strcat(temp, message);
				printf("message: %s\n", message);
				rio_writen(ipArr[userIndex], temp, strlen(temp));
				printf("AFTER SEND: %s\n", temp);
			}
		//}
		}
	}
}
	printf("%s left \n", userArr[index]);
  strcpy(userArr[index], " " );
	ipArr[index] = 0;
	numClients--;

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
