#include<stdio.h>
#include <unistd.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAX 1024

	int main()
	{
		int fd;
		char Ubuff[MAX]="Hello Device";
		fd=open("/dev/Mydevices",O_RDWR,S_IRWXU);
		if(fd < 0)
		{
			perror("Error in creating the device:");
			exit(EXIT_FAILURE);
		}
		write(fd,Ubuff,sizeof(Ubuff));
		printf("WRITE: %s\n",Ubuff);
		read(fd,Ubuff,sizeof(Ubuff));
		printf("READ: %s\n",Ubuff);
		return 0;

	}
