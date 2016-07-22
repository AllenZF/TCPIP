#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void read_childproc(int sig)
{
	int status;
	pid_t pid = waitpid(-1, &status, WNOHANG);
	if(WIFEXITED(status))
	{
		printf("Removed proc fid : %d \n", pid);
		printf("child send : %d \n", WEXITSTATUS(status));
	}
}

int main(int argc, char const *argv[])
{
	pid_t pid;
	struct sigaction act;

	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(SIGCHLD, &act, 0);

	pid = fork();
	if(pid == 0)
	{
		puts("Hi! I'm child process 1 ");
		sleep(10);
		return 12;
	}
	else
	{
		printf("child proc pid 1 : %d \n", pid);
		pid = fork();
		if(pid == 0)
	    {
			puts("Hi! I'm child process 2 ");
			sleep(10);
			return 12;
	    }
	    else
	    {
	    	int i;
	    	printf("child proc pid 2 : %d \n", pid);
	    	for(i=0;i<5;i++)
	    	{
	    		puts("wait...");
	    		sleep(5);
	    	}
	    }

	}
	return 0;
}