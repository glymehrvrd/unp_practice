#include "stdio.h"
#include "unistd.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "stdlib.h"
#include "string.h"

char inbuf[255];
char outbuf[255];

int main(){
	int sockfd;
	int recv_len;
	
	sockfd = socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=20177;
	addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	
	connect(sockfd,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));

	for(;;){
		if(fgets(outbuf,255,stdin)==NULL){
			printf("closed by user.\n");
			exit(0);
		}
		write(sockfd,outbuf,strlen(outbuf));

		if((recv_len=read(sockfd,inbuf,254))==0){
			printf("closed by server.\n");
			exit(0);
		}
		inbuf[recv_len]='\0';
		printf("%s",inbuf);
	}
}
