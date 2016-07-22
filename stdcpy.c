#include <stdio.h>

#define BUF_SIZE 3

int main(int argc, char const *argv[])
{
	FILE * fd1;
	FILE * fd2;
	
	char buf[BUF_SIZE];

	fd1 = fopen("copy.bin","r");
	fd2 = fopen("cpy.bin","w");

	while(fgets(buf,BUF_SIZE,fd1) != NULL)
	{
		fputs(buf,fd2);
	}

	fclose(fd1);
	fclose(fd2);
	return 0;
}