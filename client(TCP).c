#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int  sock;

	char buf[BUF_SIZE];
 	char filename[BUF_SIZE];	//file name

	FILE *fp;
	int read_cnt;
	int len;

	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_sz;

	if(argc!=3){
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	printf("Input File Name: ");
	gets(filename);
	printf("filename: %s\n",filename);
	int i;
	for (i = strlen(filename)+1; i < BUF_SIZE; i++){
		filename[i] = '0';
	}
	filename[BUF_SIZE - 1] = '\0';

	sock=socket(AF_INET,SOCK_STREAM,0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error!");
	else
		printf("Connected...........\n");

	fp=fopen(filename,"r");	//file open!
	if(fp==NULL)
	{
		printf("File not Found\n");
	}
	else
	{

	len=strlen(filename);
	send(sock,filename,BUF_SIZE,0);	//send the filename
	printf("filename: %s\n",filename);
	printf("filesize: %d\n",len+1);

	while((len=fread(buf,sizeof(char),BUF_SIZE,fp))>0)
	{
	//printf("len: %d\n",len);

	if(send(sock,buf,len,0)<=0)
	{
		printf("Send File Failed\n");
		break;
	}
	}

}
	fclose(fp);
	close(sock);


	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
