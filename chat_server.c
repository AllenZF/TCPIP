#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>

#define BUF_SIZE 100
#define MAX_CLNT 256

void * handle_clnt(void * arg);
void send_msg(char * msg, int len);
void error_handling(char * msg);

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutx;

int main(int argc, char const *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_sz;
	pthread_t t_id;
	int option ;

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	pthread_mutex_init(&mutx, NULL);
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);

	option = 1;
	setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&option, sizeof(option));

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind err");
    if(listen(serv_sock, 5) == -1)
    	error_handling("listen err");

    while(1)
    {
    	clnt_adr_sz =sizeof(clnt_adr);
    	clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);

    	pthread_mutex_lock(&mutx);
    	clnt_socks[clnt_cnt++]=clnt_sock;
    	pthread_mutex_unlock(&mutx);

    	pthread_create(&t_id, NULL, handle_clnt, (void *)&clnt_sock);
    	pthread_detach(t_id);
    	printf("Connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));
    }
    
    close(serv_sock);
	return 0;
}

void * handle_clnt(void * arg)
{
	int clnt_sock = *((int*)arg);
	int str_len = 0, i;
	char msg[BUF_SIZE];

	while((str_len = read(clnt_sock, msg, sizeof(msg))) != 0)
		send_msg(msg, str_len);

	pthread_mutex_lock(&mutx);
	for (i = 0; i < clnt_cnt; ++i)//remove disconnected client
	{
		if(clnt_sock == clnt_socks[i])
		{
			for (int j = 0; j < clnt_cnt; ++j)//print 
			{
				printf(" %d j=%d ", clnt_socks[j],j);
			}
           puts("\n");
           printf("i = %d \n", i);
			while( i < clnt_cnt - 1)
			{
                printf("clnt_cnt = %d \n", clnt_cnt);
				printf("i = %d \n", i);
				clnt_socks[i] = clnt_socks[i+1];
				i++;
			}

			for (int j = 0; j < clnt_cnt; ++j)
			{
				printf(" %d j=%d ", clnt_socks[j],j);
			}
			puts("\n");
			break;
		}
	}
	clnt_cnt--;
	pthread_mutex_unlock(&mutx);
	close(clnt_sock);
	return NULL;
}

void send_msg(char *msg, int len)//send to all
{
	int i;
	pthread_mutex_lock(&mutx);
	for(i=0; i<clnt_cnt;i++)
		write(clnt_socks[i],msg,len);
    pthread_mutex_unlock(&mutx);
    return ;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
