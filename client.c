#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
 
/*
���ӵ��������󣬻᲻ͣѭ�����ȴ����룬
����quit�󣬶Ͽ��������������
*/
 
int main(int argc, char ** argv) {
 
    //�ͻ���ֻ��Ҫһ���׽����ļ������������ںͷ�����ͨ��
    int clientSocket;
 
    //������������socket
    struct sockaddr_in serverAddr;
    char sendbuf[200];
    char recvbuf[200];
    int iDataNum;
 
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return 1;
 
    }
 
 	if (!argv[1] || !argv[2]) {
 		perror("cmd: ./cli hostname port");
 		return 1;
	}
 
    serverAddr.sin_family = AF_INET;
    //inet_addr()�����������ʮ����IPת���������ֽ���IP
    serverAddr.sin_addr.s_addr = inet_addr(atoi(argv[1]));
    serverAddr.sin_port = htons(atoi(argv[2]));
    if (connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
 
        perror("connect");
 
        return 1;
 
    }
 
    printf("���ӵ�����...\n");
    while (1) {
        printf("������Ϣ:");
        scanf("%s", sendbuf);
        printf("\n");
        send(clientSocket, sendbuf, strlen(sendbuf), 0);
        if (strcmp(sendbuf, "quit") == 0)
            break;
 
        printf("��ȡ��Ϣ:");
        recvbuf[0] = '\0';
        iDataNum = recv(clientSocket, recvbuf, 200, 0);
        recvbuf[iDataNum] = '\0';
        printf("%s\n", recvbuf);
 
    }
 
    close(clientSocket);
    return 0;
 
}
