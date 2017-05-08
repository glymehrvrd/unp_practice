#include "stdio.h"
#include "unistd.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "stdlib.h"
#include "string.h"

char inbuf[255];

int main(){
	int sockfd;
	int accfd;
	int recv_len;
	
	sockfd = socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=20177;
	addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	
	bind(sockfd,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
	listen(sockfd,5);

	for(;;){
		accfd = accept(sockfd,NULL,NULL);

		if(fork()!=0){
			close(accfd);
		} else {
			close(sockfd);
			for(;;){
				if((recv_len=read(accfd,inbuf,255))==0){
					printf("closed by user.\n");
					exit(0);
				}
				write(accfd,inbuf,recv_len);
			}
		}
	}
}
