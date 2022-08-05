#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

//void exchange (int client_socket){

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

	exit(0);
}