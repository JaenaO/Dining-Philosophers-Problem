#include <atomic>

using namespace std;

class PetersonLock{
  atomic flag[2] = {false, false};
  atomic turn = 0;

  void lock(int pid){
    flag[pid] = true;
    turn = pid - 1;

    while(flag[pid-1] && turn == pid-1) 
      ;  // spin
  }

  void unlock(int pid){
    flag[pid] = false;
  }
};