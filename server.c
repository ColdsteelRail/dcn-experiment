#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h> 

int main(int argc, char ** argv)
{
    int server_sockfd, client_sockfd;
    int server_len, client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    int result;
    
 	if ( !argv[1]) {
		perror("cmd: ./serv port");
		return 1;
	}
    
    fd_set readfds, testfds;
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);//������������socket 
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("121.168.56.101");
    server_address.sin_port = htons(atoi(argv[1]));
    server_len = sizeof(server_address);
    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
    listen(server_sockfd, 5); //���������������5�� 
    FD_ZERO(&readfds);
    FD_SET(server_sockfd, &readfds);//����������socket���뵽������
    while(1)
    {
        char ch;
        int fd;
        int nread;
        testfds = readfds;//����Ҫ���ӵ���������copy��select��ѯ�����У�select������޸ģ�����һ��Ҫ�ֿ�ʹ�ñ��� 
        printf("server waiting\n");
 
        /*�������������������ļ��������䶯 */
        result = select(FD_SETSIZE, &testfds, (fd_set *)0,(fd_set *)0, (struct timeval *) 0); //FD_SETSIZE��ϵͳĬ�ϵ�����ļ�������
        if(result < 1)
        {
            perror("server5");
            exit(1);
        }
 
        /*ɨ�����е��ļ�������*/
        for(fd = 0; fd < FD_SETSIZE; fd++)
        {
            /*�ҵ�����ļ�������*/
            if(FD_ISSET(fd,&testfds))
            {
                /*�ж��Ƿ�Ϊ�������׽��֣������ʾΪ�ͻ��������ӡ�*/
                if(fd == server_sockfd)
                {
                    client_len = sizeof(client_address);
                    client_sockfd = accept(server_sockfd,
                                           (struct sockaddr *)&client_address, &client_len);
		    int quickack = 1;
	            setsockopt(client_sockfd, IPPROTO_TCP, TCP_QUICKACK, &quickack, sizeof(int));

               	    int nodelay = 1;
                    setsockopt(client_sockfd, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof(int));
 
                    int srt = 1;
                    setsockopt(client_sockfd, SOL_SOCKET, 61 , &srt, sizeof(int));

                    int logme = 1;
                    setsockopt(client_sockfd, SOL_SOCKET, 62, &logme, sizeof(int));



                    FD_SET(client_sockfd, &readfds);//���ͻ���socket���뵽������
                    printf("adding client on fd %d\n", client_sockfd);
                }
                    /*�ͻ���socket������������ʱ*/
                else
                {
                    ioctl(fd, FIONREAD, &nread);//ȡ������������nread
 
                    /*�ͻ�����������ϣ��ر��׽��֣��Ӽ����������Ӧ������ */
                    if(nread == 0)
                    {
                        close(fd);
                        FD_CLR(fd, &readfds); //ȥ���رյ�fd
                        printf("removing client on fd %d\n", fd);
                    }
                        /*����ͻ���������*/
                    else
                    {
                        read(fd, &ch, 1);
                        sleep(5);
                        printf("serving client on fd %d\n", fd);
                        ch++;
                        write(fd, &ch, 1);
                    }
                }
            }
        }
    }
 
    return 0;
}
