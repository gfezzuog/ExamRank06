#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/netinet.h>

typedef struct s_client
{
	int id;
	char msg[10000];
} t_client

fd_set readfd, writefd, current;
struct t_clients clients[1024];
int maxfd = 0, gid = 0;
char send_buffer[100000], recv_buffer[100000];


void err(char *msg)
{
	if(msg)
		write(1, msg, sizeof(msg));
	else
		write(1, "Fatal error", 11);
	write(1, "\n", 1);
	exit (1);
}

void send_to_all(int except)
{
	for(int fd = 0; fd <= maxfd; fd++)
	{
		if(FD_ISSET(fd, &writefd) && fd != except)
			if(send(fd, send_buffer, strlen(send_buffer), 0) == -1)
				err(NULL);
	}
}

int main (int argc, char **argv)
{
	if (argc != 2)
		err("Wrong number of arguments");
	struct sockaddr_in serveraddr;
	socket_len len;
	int serverfd = socket(AF_INET, sock_stream, 0);
	maxfd = serverfd;

	FD_ZERO(&current);
	FD_SET(serverfd, &current);
	bzero(clients, sizeof(clients));
	bzero(serveraddr, sizeof(serveraddr));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(atoi(argv[1]));

	if(bind(serverfd, (const struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1 || listen(serverfd, 100) == -1)
		err(NULL);
	while(1)
	{
		readfd = writefd = current;
		if(select(maxfd + 1, &readfd, &writefd, 0, 0) == -1)
			continue;
		for(int fd = 0; fd <= maxfd; fd++)
		{
			if(FD_ISSET(fd, &readfd))
			{
				int clientfd = accept(serverfd, (struct sockaddr *)&serveraddr, &len);
				if (clienetfd == -1)
					continue;
				clients[clientfd].id = gid++;
				FD_SET(clientfd, &current);
				sprintf(send_buffer, server: "client %d just arrived\n", clients[clientfd].id);
				sent_to_all(clientfd);
			}
			else
			{
				int ret = recv(fd, recv_buffer, sizeof(recv_buffer), 0);
				if (ret <= 0)
				{
					sprintf(send_buffer, "server: client %d just left\n", clients[fd].id);
					send_to_all(fd);
					FD_CLR(fd, &current);
					close(fd);
				}
				else
				{
					for(int i = 0, j = strlen(clients[fd].msg; i < ret; i++, j++))
					{
						clients[fd].msg[j] = recv_buffer[i];
						if(clients[fd].msg[j] == '\n')
						{
							clients[fd].msg[j] = '\0';
							sprintf(send_buffer, "client %d: %s\n", clients[fd].id, clients[fd].msg);
							send_to_all(fd);
							bzero(clients[fd].msg, sizeof(clients[fd].msg));
							j = -1;
						}
					}	
				}
			}
			break;
		}
	}
	return (0);
}
