// Katy Soddy

#include <atomic>
#include <thread>
#include <random>
#include <fstream>
#include <string>
#include <cmath>
#include <sys/ipc.h>
#include <sys/shm.h> //int shmget(key_t key, size_t size, int shmflg);
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <cstdlib> // For exit()
using namespace std;

class finePeterLock
{
private:
    atomic<bool> flag[2];
    atomic<int> turn;

public:
    finePeterLock() : turn(0) // Initialize atomic<int> directly
    {
        flag[0].store(false); // Initialize each atomic<bool> individually
        flag[1].store(false);
    }

    void lock(int pid)
    {
        flag[pid].store(true);
        turn.store(1 - pid);
        while (flag[1 - pid] && turn != pid)
            ;
    }

    void unlock(int pid)
    {
        flag[pid].store(false);
    }
};