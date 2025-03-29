#ifndef PETERSONS_ALGORITHM_H
#define PETERSONS_ALGORITHM_H

#include <atomic>

using namespace std;

class PetersonLock{
  public:
    atomic<bool> flag[2] = {false, false};
    atomic<int> turn = 0;

    void lock(int pid){
      flag[pid] = true;
      turn = 1 - pid;

      while(flag[1-pid] && turn != pid) // other wants and their turn
        ;  // spin
    }

    void unlock(int pid){
      flag[pid] = false;
    }
};

#endif