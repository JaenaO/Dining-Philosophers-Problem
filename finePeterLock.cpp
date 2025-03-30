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
    atomic<int> lockStatus;

public:
    finePeterLock() : turn(0) // Initialize atomic<int> directly
    {
        flag[0].store(false); // Initialize each atomic<bool> individually
        flag[1].store(false);
        lockStatus.store(0);
    }

    void lock(int process) // process = i, process+1 = j
    {
        lockStatus.store(1);
        flag[process].store(true);
        turn.store(process + 1);
        while (flag[process + 1] && turn != process + 1)
            ;
    }

    void unlock(int process)
    {
        flag[process].store(false);
        lockStatus.store(0);
    }

    int checkLockStatus()
    {
        return lockStatus;
    }
};