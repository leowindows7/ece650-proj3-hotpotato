#include <netdb.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <arpa/inet.h>
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
    int num_hops = atoi(argv[3]);
    //std::cout << "Waiting for connection on port " << argv[1] << std::endl;
    std::cout << "Potato Ringmaster" << argv[1] << std::endl;
    std::cout << "Players = " << num_players << std::endl;
    std::cout << "Hops = " << num_hops << std::endl;
    
    struct sockaddr_storage socket_addr;
    socklen_t socket_addr_len = sizeof(socket_addr);
    std::vector<Player> players_vec;
    int players_connection_fd;
    std::string playerID;
    //char buffer[512];
    Player player_info;
    while (players_vec.size() < num_players)
    {
        players_connection_fd = accept(socket_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);
        struct sockaddr_in * addr = (struct sockaddr_in *)&socket_addr;
        std::string  ip(inet_ntoa(addr->sin_addr));
        std::cout << ip << std::endl;
        if (players_connection_fd == -1)
        {
            std::cerr << "Error: cannot accept connection on socket" << std::endl;
            return -1;
        }
        if (recv(players_connection_fd, &player_info, sizeof(player_info), 0) != -1)
        {   
            player_info.my_fd = players_connection_fd;
            player_info.seqNo = players_vec.size() + 1;
            players_vec.push_back(player_info);
            std::cout << "player "<< players_vec.size() << " connected to master" << std::endl;
        }
    }

    //buffer[9] = 0;
    
    //std::cout << "Server send: " << received_potato.game_progress << std::endl;
    for (int i = 0; i < players_vec.size(); i++){
        int left_fd_index = (i - 1 + players_vec.size()) % players_vec.size();
        players_vec[i].left_fd = players_vec[left_fd_index].my_fd;
        int right_fd_index = (i + 1 + players_vec.size()) % players_vec.size();
        players_vec[i].right_fd = players_vec[right_fd_index].my_fd;
        send(players_vec[i].my_fd, &players_vec[i], sizeof(players_vec[i]), 0);
    }
    Potato received_potato;
    std::cout << "Server received: " << received_potato.game_progress << std::endl;
    received_potato.game_progress[2] = 'l';
    received_potato.game_progress[3] = 'a';
    received_potato.game_progress[4] = 0;
    close(socket_fd);
    return 0;
}