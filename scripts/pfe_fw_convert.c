
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>


int main(int argc, char *argv[])
{
	int rfd, wfd;
	unsigned char rch;
	char wbuf[10];
	int count;
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

	if (argc != 3) {
		printf("Error: %s <input file>  <output file>\n", argv[0]);
		return 0;
	}

	rfd = open(argv[1], O_RDONLY);
	if (rfd < 0) {
		printf("Error : Input file(%s) does not exist\n", argv[1]);
		return 0;
	}

	wfd = open(argv[2], O_CREAT|O_WRONLY, mode);
	if (wfd < 0) {
		printf("Error: Unable to create output file(%s)\n", argv[2]);
		close(rfd);
		return 0;
	}

	while(read(rfd, &rch, 1) > 0)  {
		count = sprintf(wbuf, "0x%02x,", rch);
		count = write(wfd, wbuf, count);
	}
	
	close(rfd);
	close(wfd);

	return 0;	
}
