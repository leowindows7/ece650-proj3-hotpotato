#include <stdio.h>
#include <string.h>
#include <string>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <vector>
class Potato {
 public:
  int num_hops;
  int count;
  char game_progress[600];
  Potato() : num_hops(0), count(0) { memset(game_progress, 0, sizeof(game_progress)); }
  Potato(const Potato &rhs) :  num_hops(rhs.num_hops), count(rhs.count){memset(game_progress, 0, sizeof(game_progress));}
  Potato & operator= (const Potato &rhs){
    if(this != &rhs){
      num_hops = rhs.num_hops;
      count = rhs.count;
      memset(game_progress, 0, sizeof(game_progress));
    }
  }
  ~Potato(){};
};

class Player{
  public:
    int seqNo;
    int my_fd;
    int my_port;
    int my_left;
    int my_right;
    int num_players;
    Player(): seqNo(-1), my_fd(-1), my_port(-1), my_left(-1), my_right(-1), num_players(-1){}
    Player(const Player &rhs):seqNo(rhs.seqNo), my_fd(rhs.my_fd), my_port(rhs.my_port), my_left(rhs.my_left), my_right(rhs.my_right), num_players(rhs.num_players){}
    Player & operator=(const Player &rhs){
      if(this != &rhs){
        seqNo = rhs.seqNo;
        my_fd = rhs.my_fd;
        my_port= rhs.my_port;
        my_left = rhs.my_left;
        my_right= rhs.my_right;
        num_players = rhs.num_players;
      }
    }
    ~Player(){};
};

int start_ringmaster(const char *myPort);