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

    void lock(int pid, int p) // PROCESS MUST BE 0 OR 1 !!!!
    {
        flag[pid].store(true, memory_order_release);
        turn.store(1 - pid, memory_order_release); // Give turn to the other process
        // Wait until the other process is not interested or it's your turn
        // flag[1 - pid].load(memory_order_acquire) && turn.load(memory_order_acquire) == (1 - pid)
        while (flag[1 - pid].load(memory_order_acquire) && turn.load(memory_order_acquire) == (1 - pid))
        {
            // spin
            // cout << p << endl;
        }
        // cout << p << " here\n";
    }

    void unlock(int process)
    {
        flag[process].store(false, memory_order_release);
    }
};