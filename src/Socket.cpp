#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h> 
#include<string>


#include "Config.hpp"

using namespace std;
#define PORT 29070
extern Config cfg;

void send_stuff(std::string cmd) {
    static string prefix = "\xff\xff\xff\xffrcon " + cfg.getRconPass() + " ";
    cmd = prefix + cmd;
	char buffer[1024] = {0};
	struct sockaddr_in serv_addr{};
	static string ip =  cfg.getIP();
	static int port =  cfg.getPort();

	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	inet_pton(AF_INET, ip.c_str() , &serv_addr.sin_addr);

	int result = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(result){
        send(sockfd , cmd.c_str() , cmd.size() , 0);
        read(sockfd, buffer, 1024);
    }

}
