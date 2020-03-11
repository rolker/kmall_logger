#include <sys/socket.h>
#include <netinet/in.h>      
#include <cstring>
#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <signal.h>

// 224.1.20.40 6020 

bool done;
void signal_callback_handler(int signum)
{
    done = true;
}

int main(int argc, char **argv)
{
    done = false;
    
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    
    strftime(buffer,80,"%FT%H.%M.%S.kmall",timeinfo);
    std::cerr << buffer << std::endl;
    std::ofstream fout(buffer, std::ios::out | std::ios::binary);
    
    int listen_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    sockaddr_in listenAddress;
    memset(&listenAddress, 0, sizeof(listenAddress));
    
    listenAddress.sin_family = AF_INET;
    listenAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    listenAddress.sin_port = htons(6020);
    
    int one = 1;
    if(setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, (const char *)&one, sizeof(one)))
        std::cerr << "Error setting socket to reuse address" << std::endl;
    
    if (bind(listen_socket, (struct sockaddr *)&listenAddress, sizeof(listenAddress)) < 0)
        std::cerr << "Error binding socket" << std::endl;

    signal(SIGINT, signal_callback_handler);
    
    while(!done)
    {
        std::vector<char> data(65507); // max udp size
        sockaddr_in rx_addr;
        unsigned int rx_len = sizeof(rx_addr);
        int ret = recvfrom(listen_socket,&data.front(),data.size(),0,(struct sockaddr *)&rx_addr,&rx_len);
        if(ret>0)
            //std::cerr << "got " << ret << " bytes" << std::endl;
            fout.write(&data.front(),ret);
    }
    std::cerr << "closing file..." << std::endl;
    
    fout.close();
    return 0;
}
