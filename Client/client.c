#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXSIZE 1024

void exchange (int client_socket){

	char cmd[MAXSIZE];
	char string_msg[] = "";
	FILE *output;
	int i;

	// Loop for exchange between Client & Server
	for(;;){

		bzero(cmd, MAXSIZE);

		printf("Enter the command : ");
		fgets(cmd, MAXSIZE, stdin);
		
		printf("test : %s", cmd);
		send (client_socket, cmd, strlen(buffer), 0);

	}
}

int main(int argc, char *argv[]){

	//Configuration of IP address & port number
	int port = atoi(argv[1]);
	char *ip = argv[2];

	int client_socket;

	struct sockaddr_in addr;
	socklen_t addr_size;
	
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket < 0){
		perror ("[-] Socket error");
		exit (1);
	}

	memset(&addr, '\0', sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = port;
	addr.sin_addr.s_addr = inet_addr(ip);

	connect(client_socket, (struct sockaddr*)&addr, sizeof(addr));
	printf("[+] Connected to the server.\n");

	exchange(client_socket);

	exit(0);
}