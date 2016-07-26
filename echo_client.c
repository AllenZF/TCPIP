#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024



void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char const *argv[])
{
	int sock;
	struct sockaddr_in serv_adr;
	char message[BUF_SIZE],message1[BUF_SIZE];
	int str_len;
	
	if(argc != 3)
	{
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}
	
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
		error_handling("socket() error");

    memset(&serv_adr,0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    if(!inet_aton(argv[1], &serv_adr.sin_addr))
    	error_handling("conversion error");
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
    	error_handling("connect() error");
    else
    	puts("connected ....");


    while(1)
    {
    	fputs("Input message : ", stdout);
    	fgets(message, BUF_SIZE, stdin);

    	if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))
    	   break;//shutdown(sock,SHUT_RD);
    	write(sock,message,strlen(message));
    	str_len = read(sock,message1,BUF_SIZE-1);
    	message1[str_len]=0;
    	printf("Message from server : %s",message1);
    }

    close(sock);
    return 0;
}