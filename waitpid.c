#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h> 

int main(int argc, char const *argv[])
{
	int status;
	pid_t pid = fork();

	if(pid == 0)
	{
		sleep(15);
		exit(24) ;
	}
    else
    {
    	while(!waitpid(-1, &status, WNOHANG))
    	{
    		sleep(3);
    		puts("sleep 3 sec \n");
    	}

    	if(WIFEXITED(status))
    		printf("Child send %d \n", WEXITSTATUS(status));
    }

	return 0;
}