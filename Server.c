// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <sys/utsname.h>
#include <string.h>
#define PORT 5000

typedef struct
{
	struct sockaddr_in addr; /* Client remote address */
	int connfd;				 /* Connection file descriptor */
	int uid;				 /* Client unique identifier */
	char name[32];			 /* Client name */
} client_t;

typedef struct
{
	char OS[16];
	char valid;
} GET_LOCAL_OS;

typedef struct  
{
	int time;
	char valid;
} GET_LOCAL_TIME;


void strip_newline(char *s)
{
	while (*s != '\0')
	{
		if (*s == '\r' || *s == '\n')
		{
			*s = '\0';
		}
		s++;
	}
}

 void GetLocalTime(GET_LOCAL_TIME *dt) {
	dt = malloc(sizeof(GET_LOCAL_TIME));
	time_t seconds; 
    seconds = time(NULL); 
	dt->time = seconds;
	dt->valid = '1';
 }

 void GetLocalOS(GET_LOCAL_OS *ds) {
	ds = malloc(sizeof(GET_LOCAL_OS));
	#ifdef _WIN32 
		ds->OS = "Windows OS";
		ds->valid = '1'; 
	
	#elif __linux__ 
		ds->OS = "Linum OS";
		ds->valid = '1'; 

	#elif TARGET_OS_EMBEDDED 
		ds->OS = "iOS embedded OS";
		ds->valid = '1'; 

	#elif TARGET_IPHONE_SIMULATOR 
		ds->OS = "iOS simulator OS";
		ds->valid = '1'; 

	#elif TARGET_OS_IPHONE 
		ds->OS = "iPhone OS";
		ds->valid = '1'; 

	#elif TARGET_OS_MAC
		ds->OS = "MAC OS";
		ds->valid = '1'; 
	
	#elif__ANDROID__ 
		ds->OS = "Android OS";
		ds->valid = '1';

	#elif __unix__
		ds->OS = "Unix OS";
		ds->valid = '1'; 

	#elif __sun 
		ds->OS = "Solaris OS";
		ds->valid = '1';

	#elif __hpux 
		ds->OS = "HP UX OS";
		ds->valid = '1';

	#elif BSD 
		ds->OS = "Solaris OS";
		ds->valid = '1';

	#elif __DragonFly__ 
		ds->OS = "DragonFly BSD OS";
		ds->valid = '1';

	#elif __FreeBSD__ 
		ds->OS = "FreeBSD OS";
		ds->valid = '1';

	#elif __NetBSD__ 
		ds->OS = "Net BSD OS";
		ds->valid = '1';

	#elif __OpenBSD__ 
		ds->OS = "BSD OS";
		ds->valid = '1';

	#else 
		ds->valid = '0'; 
	#endif 
 }

int readInt(char buf[4]) {
	int num = atoi(buf);
	printf("num %d\n", num);
 	int swapped = ((num>>24)&0xff) | // move byte 3 to byte 0
                    ((num<<8)&0xff0000) | // move byte 1 to byte 2
                    ((num>>8)&0xff00) | // move byte 2 to byte 1
                    ((num<<24)&0xff000000); 
	return swapped;
}

 void *CmdProcessor(void *arg)
{
	printf("anu\n");
	char buff_out[1024];
	char buff_in[1024];
	int rlen;
	char * localtime;
	char * localos;
	int n;
	client_t *cli = (client_t *)arg;
	while ((n = recv(cli->connfd, buff_in, sizeof buff_in, 0)) > 0) {
            //fwrite(buff_in, sizeof(char), n, cli->connfd);
            printf("Received %d bytess\n", n);
			char *buffer = malloc(99 + 1);
			memcpy(buffer, buff_in, 99);
			buffer[99] = 0;
			int i;
			while(i < 100) 
			{
				printf("%c", buffer[i]);
			}
			/*int i;
			char CmdID[100];
			strncpy(CmdID, buff_in, 100);
			char CmdLength[4];
			strncpy(CmdLength, buff_in+100, 4);
			int value = readInt(CmdLength);*/
			//printf("buffer %s %s\n", htonl(buffer));
    }
	/*while (1)
	{
		
		printf("anu\n");
		rlen = read(cli->connfd, buff_in, sizeof(buff_in) - 1);
		char CmdID[100];
		strncpy(CmdID, buff_in, 100);
		printf("buffer value %s\n", ntohl(CmdID));
		printf("buffer length %d\n", rlen);
		buff_in[rlen] = '\0';
		buff_out[0] = '\0';
		strip_newline(buff_in);
		char *command = strtok(buff_in, " ");
		printf("%s\n", command);
		if (strcmp(command, "GetLocalTime") == 0) {
			GET_LOCAL_TIME *dt;
			GetLocalTime(dt);
			printf("time %d\n", dt->time);
		} else if (strcmp(command, "GetLocalOS") == 0) {
			GET_LOCAL_OS *ds;
			GetLocalOS(ds);
			printf("os %s\n", ds->OS);
		}
		//snprintf(buff_out, sizeof(buff_out), "[%s] %s\r\n", cli->name, buff_in);
		char *hello = "hello";
		//send(cli->connfd, hello, strlen(hello), 0);
		write(cli->connfd, hello, strlen(hello));
		printf("hello sent\n");
	}*/
	return NULL;
}

int main(int argc, char const *argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address, cli_address;
	int opt = 1;
	int addrlen = sizeof(cli_address);
	char buffer[1024] = {0};
	char *hello = "Over";

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	/*if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
												&opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} */
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(PORT);

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address,
			 sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	while (1)
	{

		if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
								 (socklen_t *)&addrlen)) < 0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}
		pthread_t tid;
		client_t *cli = (client_t *)malloc(sizeof(client_t));
		cli->addr = cli_address;
		cli->connfd = new_socket;
		pthread_create(&tid, NULL, &CmdProcessor, (void *)cli);
	}
	/*int i=0;
	while (i<10)
	{
		send(new_socket, hello, strlen(hello), 0);
		printf("Hello message sent\n");
		i++;
		bzero(buffer, 1024);
		valread = read( new_socket , buffer, 1024);
		printf("%s\n",buffer );
	}*/

	return 0;
}
