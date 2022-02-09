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
    close(socket_fd);
    return 0;
}