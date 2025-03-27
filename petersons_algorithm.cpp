#include <atomic>

using namespace std;

class PetersonLock{
  public:
    atomic flag[2] = {false, false};
    atomic turn = 0;

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