// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
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

 GET_LOCAL_TIME* GetLocalTime(GET_LOCAL_TIME *dt) {
	dt = malloc(sizeof(GET_LOCAL_TIME));
	time_t seconds; 
    seconds = time(NULL); 
	dt->time = seconds;
	dt->valid = '1';
	return dt;
 }

 GET_LOCAL_OS* GetLocalOS(GET_LOCAL_OS *ds) {
	ds = malloc(sizeof(GET_LOCAL_OS));
	struct utsname name;
	if(uname(&name)) exit(-1);
	strncpy(ds->OS, name.sysname, 16);
	ds->valid = '1';
	return ds;
 }

int receive_one_byte(int client_socket, char *cur_char)
{
    ssize_t bytes_received = 0;
	while (bytes_received != 1)
	{
		bytes_received = recv(client_socket, cur_char, 1, 0);
	} 
	
	return 1;
}

int receiveFully(int client_socket, char *buffer, int length)
{
	char *cur_char = buffer;
	ssize_t bytes_received = 0;
	while (bytes_received != length)
	{
	    receive_one_byte(client_socket, cur_char);
	    cur_char++;
	    bytes_received++;
	}
	
	return 1;
}

void printBinaryArray(char *buffer, int length)
{
    int i=0;
    while (i<length)
    {
		printf(" %c ", buffer[i]);
		i++;
	}
	
	printf("\n");
}

int toInteger32(char *bytes)
{
	int tmp = (bytes[0] << 24) + 
	          (bytes[1] << 16) + 
	          (bytes[2] << 8) + 
	          bytes[3];
   
	return tmp;
}

char* intToByte(int n) {
	char *bytes = (char*)malloc(4);
	bytes[0] = (n >> 24) & 0xFF;
	bytes[1] = (n >> 16) & 0xFF;
	bytes[2] = (n >> 8) & 0xFF;
	bytes[3] = n & 0xFF;
	return bytes;
}

 void *CmdProcessor(void *arg)
{
	char buff_in[1024];
	char * localtime;
	char * localos;
	int n;
	client_t *cli = (client_t *)arg;
	while ((n = recv(cli->connfd, buff_in, sizeof buff_in, 0)) > 0) {
			char cmdID[100];
			for (int i = 0; i < 100; i++) {
				cmdID[i] = buff_in[i];
			}
			GET_LOCAL_OS *ds;
			GET_LOCAL_TIME *dt;
			if (strncmp(cmdID, "GetLocalOS", 10) == 0) {
				ds = GetLocalOS(ds);
				
			} else if (strncmp(cmdID, "GetLocalTime", 12) == 0) {	
				dt = GetLocalTime(dt);
			} else {
				return NULL;
			}
	 		char packet_buf[4];
			int j = 0; 
			for (int i = 100; i < 104; i++) {
				packet_buf[j] = buff_in[i];
				j++;
			}
			int packet_len = toInteger32(packet_buf);
			char buff_out[packet_len];
			bzero(buff_out, sizeof buff_out);
			if(ds) {
				for(int i = 0; i < 16; i++) {
					buff_out[i] = ds->OS[i];
				}
				buff_out[17] = ds->valid;
			}
			if (dt) {
				char bytes[4];
				memcpy(bytes, (char*)&(dt->time),sizeof(int));
				for(int i = 0; i < 4; i++) {
					buff_out[i] = bytes[i];
				}
				buff_out[4] = dt->valid;
			}
			send(cli->connfd, buff_out, packet_len, 0);
    }
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

	return 0;
}
