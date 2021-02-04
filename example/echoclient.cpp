/*linux socket AF_INET TCP 编程示例，单进程单线程，ehco 客户端*/
#include <cstdio>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

int main()
{
	int cli_sock = socket(AF_INET, SOCK_STREAM, 0);
	
	// set server 端的 ip 地址和端口号
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(8123);										// htons: 将主机字节顺序转换成网络字节顺序
	//serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//将 inet_addr() 改用 inet_pton() 这个现代的方法，支持 IPV4 和 IPV6
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) == -1) {
		printf("inet_pton error\n");
		close(cli_sock);
		return 0;
	}
	
#if 0
	/** 功能：打印ip 地址和端口号*/
	//将 inet_ntoa() 改用 inet_ntop() 这个现代方法，支持 IPV4 和 IPV6
	//printf("bind in %s : %d\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));
	//INET_ADDRSTRLEN 是标识 IPV4 地址展现字符串的大小常量,INET6_ADDRSTRLEN是 IPV6 的
	char serv_ip[INET_ADDRSTRLEN]; 
	if (inet_ntop(AF_INET, &serv_addr.sin_addr, serv_ip, sizeof(serv_ip)) == NULL) {		// inet_ntop: 将网络传输的数值格式转换成点分十进制的ip格式
		printf("inet_ntop error\n");
		close(cli_sock);
		return 0;
	}
	printf("bind in %s : %d\n", serv_ip, ntohs(serv_addr.sin_port));	// ntohs: 将网络传输的字节顺序 转换成 本地主机的字节顺序
#endif
	
	int conn = connect(cli_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (conn == -1) {
		printf("connect error\n");
		return 0;
	}
	
	// client send data to server 
	char s[] = "hello world";
	write(cli_sock, s, strlen(s) + 1);
	
	// client receive data from socket buffer
	char buf[256];
	read(cli_sock, buf, sizeof(buf));
	printf("read %s\n", buf);
	
	close(cli_sock);
	return 0;
}
