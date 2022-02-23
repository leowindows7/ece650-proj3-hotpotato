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
    if (num_hops < 0)
    {
        std::cerr << "num of hops must be greater or equal to 0" << std::endl;
        return -1;
    }

    if (num_players < 2)
    {
        std::cerr << "num of players must be greater or equal to 2" << std::endl;
        return -1;
    }
    // std::cout << "Waiting for connection on port " << argv[1] << std::endl;
    std::cout << "Potato Ringmaster" << std::endl;
    std::cout << "Players = " << num_players << std::endl;
    std::cout << "Hops = " << num_hops << std::endl;

    struct sockaddr_storage socket_addr;
    socklen_t socket_addr_len = sizeof(socket_addr);
    std::vector<Player> players_vec;
    int players_connection_fd;
    std::string playerID;
    Player player_info;
    while (players_vec.size() < num_players)
    {

        players_connection_fd = accept(socket_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);
        struct sockaddr_in *addr = (struct sockaddr_in *)&socket_addr;
        std::string ip(inet_ntoa(addr->sin_addr));
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
            player_info.my_port = addr->sin_port;
            players_vec.push_back(player_info);
            std::cout << "Player " << players_vec.size() - 1 << " is ready to play" << std::endl;
        }
    }

    for (int i = 0; i < players_vec.size(); i++)
    {
        players_vec[i].num_players = players_vec.size();
        int left_fd_index = (i - 1 + players_vec.size()) % players_vec.size();
        int right_fd_index = (i + 1 + players_vec.size()) % players_vec.size();
        players_vec[i].my_left = players_vec[left_fd_index].seqNo;
        players_vec[i].my_right = players_vec[right_fd_index].seqNo;
        send(players_vec[i].my_fd, &players_vec[i], sizeof(players_vec[i]), 0);
    }
    if (num_hops == 0)
    {
        return -1;
    }
    Potato sent_potato;
    sent_potato.num_hops = num_hops;
    sent_potato.count = 0;
    srand(time(NULL));
    // fisrt throw from server to player
    int send_to_player = rand() % players_vec.size();
    sent_potato.game_progress[sent_potato.count] = send_to_player;
    send(players_vec[send_to_player].my_fd, &sent_potato, sizeof(sent_potato), 0);
    std::cout << "Ready to start the game, sending potato to player: " << send_to_player << std::endl;
    // fisrt throw from server to player
    std::vector<int> play_path;
    while (sent_potato.num_hops > 0)
    {
        play_path.push_back(send_to_player);
        recv(players_vec[send_to_player].my_fd, &sent_potato, sizeof(sent_potato), 0);
        send_to_player = sent_potato.game_progress[sent_potato.count];
        if (sent_potato.num_hops <= 0)
        {
            break;
        }
        send(players_vec[send_to_player].my_fd, &sent_potato, sizeof(sent_potato), 0);
    }
    std::cout << "Trace of Potato:" << std::endl;
    for (int i = 0; i < sent_potato.count; i++)
    {
        std::string delimeter = ",";
        if (i == sent_potato.count - 1)
        {
            delimeter = "";
        }
        std::cout << play_path[i] << delimeter << sent_potato.game_progress[1];
    }
    std::cout << std::endl;
    close(socket_fd);
    return 0;
}
