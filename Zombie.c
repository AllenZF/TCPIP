#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	pid_t pid = fork();
	if(pid == 0)
		printf("Hi , I am a child Process\n");
	else
	{
		printf("child Process ID : %d \n", pid);
		sleep(30);
	}

	if(pid == 0)
		puts("End child Process");
	else 
		puts("End parent Process");

	return 0;
}