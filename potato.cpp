#include "potato.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

int start_ringmaster(const char *myPort)
{
    int status;
    int socket_fd;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    const char *hostname = NULL;
    const char *port = myPort;

    memset(&host_info, 0, sizeof(host_info));

    host_info.ai_family = AF_UNSPEC;
    host_info.ai_socktype = SOCK_STREAM;
    host_info.ai_flags = AI_PASSIVE;
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

    int yes = 1;
    status = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    status = bind(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1)
    {
        std::cerr << "Error: cannot bind socket" << std::endl;
        std::cerr << "  (" << hostname << "," << port << ")" << std::endl;
        return -1;
    } //if

    status = listen(socket_fd, 100);
    if (status == -1)
    {
        std::cerr << "Error: cannot listen on socket" << std::endl;
        std::cerr << "  (" << hostname << "," << port << ")" << std::endl;
        return -1;
    } //if
    // review this part
    std::cout << "Waiting for connection on port " << port << std::endl;
    struct sockaddr_storage socket_addr;
    socklen_t socket_addr_len = sizeof(socket_addr);
    int client_connection_fd;
    client_connection_fd = accept(socket_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);
    if (client_connection_fd == -1)
    {
        std::cerr << "Error: cannot accept connection on socket" << std::endl;
        return -1;
    } //if

    //char buffer[512];
    Potato received_potato;
    recv(client_connection_fd, &received_potato, sizeof(received_potato), 0);
    //buffer[9] = 0;

    std::cout << "Server received: " << received_potato.game_progress << std::endl;
    received_potato.game_progress[2] = 'l';
    received_potato.game_progress[3] = 'a';
    received_potato.game_progress[4] = 0;
    std::cout << "Server send: " << received_potato.game_progress << std::endl;
    send(socket_fd, &received_potato, sizeof(received_potato), 0);
    freeaddrinfo(host_info_list);

    return socket_fd;
}