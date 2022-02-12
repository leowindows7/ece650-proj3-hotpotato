#include <stdio.h>
#include <string.h>

#include <cstdio>
#include <cstdlib>
class Potato {
 public:
  int num_hops;
  int count;
  char game_progress[512];
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
    int left_fd;
    int right_fd;
    Player(): seqNo(-1), my_fd(-1), left_fd(-1), right_fd(-1){}
};

int start_ringmaster(const char *myPort);