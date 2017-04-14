#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 512

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sock, new_sock;
	FILE *fp;
	struct sockaddr_in serv_adr, clnt_adr;
	pid_t pid;

	int clnt_addr_size, read_len = 0, write_len = 0;

	char buf[BUF_SIZE];
	char file_name[BUF_SIZE];

	if (argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}


	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)	//bind
		error_handling("bind() error");

	if (listen(serv_sock, 5) == -1)	//listen
		error_handling("listen() error");
	z
	while (1){
		clnt_addr_size = sizeof(clnt_adr);
		new_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_addr_size);	//accept
		if (new_sock == -1)
			error_handling("accept() error");
		else
			printf("accepted\n");

		pid = fork();	//make child process
		if (pid == -1)
		{
			close(new_sock);
			continue;
		}
		if (pid == 0)		//child process
		{
			recv(new_sock, file_name, BUF_SIZE, 0);	//receive filename
			printf("filename size: %d\n", strlen(file_name));
			printf("%c", file_name[strlen(file_name) + 2]);
			fp = fopen(file_name, "w");
			if (fp == NULL){
				printf("Cannot open to write!\n");
				exit(1);
			}
			else
				printf("opened: %s\n", &file_name);
			int i = 0;
			int j;
			while (read_len = recv(new_sock, buf, BUF_SIZE, 0))
			{
				if (i == 0){
					for (j = 0;j < BUF_SIZE; j++){
						if (buf[j] == '\0')
							break;
					}
					if (j != BUF_SIZE-1)
						fwrite(buf+j+4, sizeof(char), read_len - j, fp);
					i++;
					continue;
				}
				else
				write_len = fwrite(buf, sizeof(char), read_len, fp);
				if (write_len < read_len)
				{
					printf("no enough room\n");
					break;
				}

			}
			printf("file %s transfer success!\n", file_name);

			fclose(fp);
			close(new_sock);
		}	//end of child process
		else
			close(new_sock);
	}
	close(serv_sock);
	return 0;
}



void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
