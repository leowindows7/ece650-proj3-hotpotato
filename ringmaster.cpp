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
        //std::cout << ip <<" "<< addr->sin_port << std::endl;
        if (players_connection_fd == -1)
        {
            std::cerr << "Error: cannot accept connection on socket" << std::endl;
            return -1;
        }
        int status = recv(players_connection_fd, &player_info, sizeof(player_info), 0);
        if (status != -1)
        {   
            player_info.my_fd = players_connection_fd;
            player_info.seqNo = players_vec.size();
            //player_info.my_host= "fuck";
            player_info.my_port = addr->sin_port;
            players_vec.push_back(player_info);
            std::cout << "player "<< players_vec.size() - 1 << " connected to master" << std::endl;
        }
    }

    //buffer[9] = 0;
    
    //std::cout << "Server send: " << received_potato.game_progress << std::endl;
    for (int i = 0; i < players_vec.size(); i++){
        int left_fd_index = (i - 1 + players_vec.size()) % players_vec.size();
        int right_fd_index = (i + 1 + players_vec.size()) % players_vec.size();
        players_vec[i].my_left = players_vec[left_fd_index].seqNo;
        players_vec[i].my_right = players_vec[right_fd_index].seqNo;
        //players_vec[i].nei_host.push_back(players_vec[right_fd_index].my_host[0]);
        send(players_vec[i].my_fd, &players_vec[i], sizeof(players_vec[i]), 0);
        //std::cout << players_vec[i].my_fd << std::endl;
    }
    Potato sent_potato;
    sent_potato.num_hops = num_hops;
    sent_potato.count = 0;
    srand (time(NULL));
    //fisrt throw from server to player
    int send_to_player = rand() % players_vec.size();
    sent_potato.game_progress[sent_potato.count] = send_to_player;
    send(players_vec[send_to_player].my_fd, &sent_potato, sizeof(sent_potato), 0);
    std::cout << "Server send potato to: " << send_to_player << std::endl;
    //fisrt throw from server to player
    while(sent_potato.num_hops > 0){
        std::cout << "num_hop: " << sent_potato.num_hops << std::endl;
        recv(players_vec[send_to_player].my_fd, &sent_potato, sizeof(sent_potato), 0);
        send_to_player = sent_potato.game_progress[sent_potato.count];
        //std::cout << "count: " << sent_potato.count << std::endl;
        send(players_vec[send_to_player].my_fd, &sent_potato, sizeof(sent_potato), 0);
        std::cout << "Server send potato to: " << send_to_player << std::endl;
    }
    close(socket_fd);
    return 0;
}
