#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 2500
#define MAX_CONNECTIONS 10

void Eroare(char *e) 
{ 
	perror(e); 
	exit(1); 
}



struct sockaddr_in newAddr;
int newSocket;

int main(){

	int sock_file_des, bind_return;
	struct sockaddr_in serverAddr;
	struct sockaddr_in newAddr;
	socklen_t addr_size;

	char buffer[102400];
	pid_t childpid;

	sock_file_des = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_file_des < 0){
		printf("Connection Error.\n");
		exit(1);
	}
	printf("Server Socket created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	bind_return = bind(sock_file_des, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(bind_return < 0){
		printf("Binding error.\n");
		exit(1);
	}
	printf("Bind to port %d\n", 2500);

	if(listen(sock_file_des, MAX_CONNECTIONS) == 0){
		printf("Waiting for Client....\n");
	}else{
		printf("Binding error.\n");
	}


	while(1){
		newSocket = accept(sock_file_des, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
		//printf("Connection accepted");
		if((childpid = fork()) == 0){
			close(sock_file_des);
			dup2( newSocket, STDOUT_FILENO );  /* duplicate socket on stdout */
    		dup2( newSocket, STDERR_FILENO );  /* duplicate socket on stderr */
			
			clientCommand(buffer);
		}
	}
	close(newSocket);
	return 0;
}

void clientCommand(char *buffer){
	while(1){
				recv(newSocket, buffer, 102400, 0);
				if(strcmp(buffer, "quit") == 0){
					break;
				}

				else{
					system(buffer);					
					bzero(buffer, sizeof(buffer));
				}
	}
}
