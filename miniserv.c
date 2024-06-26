#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet.h>

typedef struct s_client
{
	int id;
	char msg[10000];
}	t_client;

t_client clients[1024];
fd_set read_fd, write_fd, current;
int maxfd = 0, gid = 0;
char send_buffer[120000], recv_buffer[120000];

void err(char *msg)
{
	if(msg)
		write(2, msg, strlen(msg));
	else
		write(2, "Fatal error", 11);
	write(1, "\n", 1);
	exit 1;
}

void send_to_all(int except)
{
	for(int fd = 0; fd<=maxfd; fd++)
	{
		if (FD_ISSET(fd, &write_fd) && fd != except)
			if(send(fd, send_buffer, strlen(send_buffet), 0) == -1)
				err(NULL);
	}
}

inf main(int argc, char **argv)
{
	if (ac != 2)
		err("Wrong number of arguments");

	struct sockaddr_in serveraddr;
	socklen_t len;

	int serverdfd = socket(AF_INET, SOCK_STREAM, 0);
	if(serverfd == -1)
		err(NULL);
	maxfd = serverfd;
	FD_ZERO(&current);
	FD_SET(serverfd, &current);
	bzero(clients, sizeof(clients));
	bzero(&serveraddr, sizeof(serveraddr));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(atoi(argv[1]));

	if(bind(serverfd, (const struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1 || listen(serverfd, 100) == -1)
		err(NULL);
	while(1)
	{
		read_fd = write_fd = current;
		if(select(maxfd + 1, &read_fd, &write_fd, 0, 0) == -1)
			continue;
		for(int fd = 0; fd <= maxfd; fd++)
		{
			if(FD_ISSET(fd, &read_fd))
			{
				if(fd == serverfd)
				{
					int clientfd = accept(serverfd, (struct sockaddr *)&serveraddr, &len);
					if (clientfd == -1)
						continue;
					if(clientfd > maxfd)
						maxfd = clientfd;
					clients[clientfd].id == gid++;
					FD_SET(clientfd, &current);
					sprintf(send_buffer, "server: client &d just arrived\n", clients[clientfd].id);
					send_to_all(clientfd);
			
				}
				else
				{
					int ret = recv(fd, recv_buffer, sizeof(recv_buffer), 0);
					if(ret <= 0)
					{
						send_to_all(fd);
						FD_CLR(fd, &current);
						close(fd);
				
					}
					else
					{
						for(int i = 0, j = strlen(clients[fd].msg); i < ret; i++, j++)
						{
							clients[fd].msg[j] = recv_buffer[i];
							if(clients[fd].msg[j] == '\n')
							{
								clients[fd].msg[j] == '\0';
								sprintf(send_buffer, "client %d: %s\n", clients[fd].id, clientd[fd].msg);
								send_to_all(fd);
								bzero(clientd[fd].msg, strlen(clientd[fd.msg]));
								j = -1;
							}
						}
					}
				}
				break;
			}
		}
		return(0);
	}















}
