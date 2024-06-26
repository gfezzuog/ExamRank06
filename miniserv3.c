#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/netinet.h>

typedef struct s_client
{
	int id;
	char msg[100000];
} t_client

fd_set readfd, writefd, current;
int maxfd = 0, gid = 0;
char send_buffer[120000], recv_buffer[120000];
t_client clients[1024];

void err(char *msg)
{
	if(msg)
		write(1, msg, sizeof(msg));
	else
		write(1, "Fatal error", 11);
	write(1, "\n", 1);
	exit(1);
}

void sent_to_all(int except)
{
	for(int fd = 0; fd <= maxfd; fd++)
	{
		if(FD_ISSET(fd, &writefd))
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
	int serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverfd == -1)
		err(NULL);

	maxfd = serverfd;
	FD_ZERO(&current);
	FD_SET(serverfd, &current);
	bzero(clients, sizeof(clients));
	bzero(serveraddr, sizeof(serveraddr));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(atoi(argv[1]));

	while(1)
	{
		readfd = writefd = current;
		if(select(maxfd + 1, &readfd, &writefd, 0, 0) == -1)
			continue;
		for(fd = 0; fd <= maxfd; fd++)
		{
			if(FD_ISSET(fd, &readfd))
			{
				if(fd == serverfd)
				{
					int clientfd = accept(serverfd, (const struct sockaddr *)&serveraddr, &len);
					if(clientfd == -1)
						continue;
					if (maxfd < clientfd)
						maxfd = clientfd;
					clients[clientfd].id = gid++;
					FD_SET(clientfd, &current);
					sprintf(send_buffer, "server: client %d just arrived\n", clients[clientfd].id);
					send_to_all(clientfd);
				}
				else
				{
					int ret = recv(fd, 
				}
			}
		}
	}
}
