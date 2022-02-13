#include <assert.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <vector>

#include "potato.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "Syntax: input player <machine_name> <port_num>" << std::endl;
        return 1;
    }

    int status;
    int socket_fd;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    const char *hostname = argv[1]; //152.3.76.77
    const char *port = argv[2];// 4444

    memset(&host_info, 0, sizeof(host_info));
    host_info.ai_family = AF_UNSPEC;
    host_info.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(hostname, port, &host_info, &host_info_list);
    if (status != 0)
    {
        std::cerr << "Error: cannot get address info for host" << std::endl;
        std::cerr << "  (" << hostname << "," << port << ")" << std::endl;
        return -1;
    } //if

    socket_fd = socket(host_info_list->ai_family,
                       host_info_list->ai_socktype,
                       host_info_list->ai_protocol);
    if (socket_fd == -1)
    {
        std::cerr << "Error: cannot create socket" << std::endl;
        std::cerr << "  (" << hostname << "," << port << ")" << std::endl;
        return -1;
    } //if

    std::cout << "Connecting to " << hostname << " on port " << port << "..." << std::endl;

    status = connect(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1)
    {
        std::cerr << "Error: cannot connect to socket" << std::endl;
        std::cerr << "  (" << hostname << "," << port << ")" << std::endl;
        return -1;
    } //if

    // above connect to ringmaster 

    const char *message = "hi there!";
    Potato play_potato;
    Player myInfo;
    //send(socket_fd, message, strlen(message), 0);
    send(socket_fd, &myInfo, sizeof(myInfo), 0);
    recv(socket_fd, &myInfo, sizeof(myInfo), 0);
    //std::cout << "Myport: " << myInfo.my_port << std::endl;
    std::cout << "My left: " << myInfo.my_left << std::endl;
    std::cout << "My right: " << myInfo.my_right << std::endl;
    status = recv(socket_fd, &play_potato, sizeof(myInfo), 0);
    if (status > 0){
        std::cout << "I received a potato!" << std::endl;
        play_potato.num_hops--;
        play_potato.path.push_back(myInfo.my_right);
        play_potato.count++;
        play_potato.game_progress[play_potato.count] = myInfo.my_right;
        send(socket_fd, &play_potato, sizeof(myInfo), 0);
    }
    freeaddrinfo(host_info_list);
    close(socket_fd);
    return 0;
}