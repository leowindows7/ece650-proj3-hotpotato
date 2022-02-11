#include <netdb.h>
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
    if (argc != 4)
    {
        std::cout << "Syntax: ringmaster <port_num> <num_players> <num_hops>" << std::endl;
        return 1;
    }
    int socket_fd = start_ringmaster(argv[1]);
    int num_players = atoi(argv[2]);
    std::cout << "Waiting for connection on port " << argv[1] << std::endl;
    struct sockaddr_storage socket_addr;
    socklen_t socket_addr_len = sizeof(socket_addr);
    std::vector<int> players_fd;
    int players_connection_fd;

    //char buffer[512];
    Potato received_potato;
    int player_count = 0;
    while (players_fd.size() < num_players)
    {
        players_connection_fd = accept(socket_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);
        if (players_connection_fd == -1)
        {
            std::cerr << "Error: cannot accept connection on socket" << std::endl;
            return -1;
        }
        if (recv(players_connection_fd, &received_potato, sizeof(received_potato), 0) != -1)
        {   
            players_fd.push_back(players_connection_fd);
            std::cout << "player "<< players_fd.size() << " connected" << std::endl;
        }
    }

    //buffer[9] = 0;

    std::cout << "Server received: " << received_potato.game_progress << std::endl;
    received_potato.game_progress[2] = 'l';
    received_potato.game_progress[3] = 'a';
    received_potato.game_progress[4] = 0;
    std::cout << "Server send: " << received_potato.game_progress << std::endl;
    for (int i = 0; i < players_fd.size(); i++){
        send(players_fd[i], &received_potato, sizeof(received_potato), 0);
    }
    
    close(socket_fd);
    return 0;
}