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
    atomic<bool> flag[2]; // does it want to enter critical section
    atomic<int> turn;     // whose turn is it

public:
    finePeterLock() : turn(0) // Initialize atomic<int> directly
    {
        flag[0].store(false, memory_order_relaxed); // Initialize each atomic<bool> individually
        flag[1].store(false, memory_order_relaxed);
        turn.store(0, memory_order_relaxed);
    }

    void lock(int process) // process = i, process+1 = j
    {
        int other = 1 - process;
        flag[process].store(true, memory_order_relaxed);
        turn.store(other, memory_order_relaxed);
        // wait until other process is done
        while (flag[other].load(memory_order_acquire) && turn.load(memory_order_acquire) == other)
        {
            // spin
        }
    }

    void unlock(int process)
    {
        flag[process].store(false, memory_order_release);
    }
};