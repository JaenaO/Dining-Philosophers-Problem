#include <iostream>
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
#include <mutex>
#include "finePeterLock.cpp"
using namespace std;

mutex cout_mutex;

// jaena's random_ms() function from coarse.cpp
chrono::milliseconds random_ms()
{
    // code from websites provided in instructions pdf
    random_device rd;                           // a seed source for the random number engine
    mt19937 gen(rd());                          // mersenne_twister_engine seeded with rd()
    uniform_int_distribution<> distrib(1, 500); // range of random numbers 1-500
    return chrono::milliseconds(distrib(gen));
}

// print without interruptions
void safe_print(const string &message)
{
    lock_guard<mutex> guard(cout_mutex);
    cout << message << endl;
}

void thinking(int id)
{
    safe_print("Philosopher " + to_string(id + 1) + ": starts thinking");
    this_thread::sleep_for(random_ms());
    safe_print("Philosopher " + to_string(id + 1) + ": ends thinking");
}

void eating(int id)
{
    safe_print("Philosopher " + to_string(id + 1) + ": starts eating");
    this_thread::sleep_for(random_ms());
    safe_print("Philosopher " + to_string(id + 1) + ": ends eating");
}

void philosopher(int id, int n, finePeterLock *chopsticks)
{
    thinking(id);

    // if (id == 0)
    // {
    //     chopsticks[0].lock(0, 0);
    // }

    // if (id == 1)
    // {
    //     chopsticks[0].lock(1, 1);
    // }

    while (true)
    {
        // Uses the third attempt algorithm from the slides
        if (id < n - 1)
        {
            chopsticks[id].lock(1, id);
            chopsticks[(id + 1) % n].lock(0, id);
        }
        else // wrap around
        {
            chopsticks[(id + 1) % n].lock(0, id);
            chopsticks[id].lock(1, id);
        }
        // critical section
        eating(id);
        // release
        chopsticks[id].unlock(1);
        chopsticks[(id + 1) % n].unlock(0);
        break;
    }
}

int main(int argc, char *argv[])
{
    cout << "---------- Fine Solution ---------\n\n\n";
    if (argc < 2 || atoi(argv[1]) < 2)
    {
        cout << "Usage: " << argv[0] << " <n philosophers (at least 2)>" << endl;
        return 1;
    }

    int n = atoi(argv[1]);

    // initialize locks
    // vector<finePeterLock> chopsticks(n);
    // for (int i = 0; i < n; i++)
    // {
    //     chopsticks[i] = new finePeterLock();
    // }

    vector<finePeterLock> chopsticks(n);
    vector<thread> philosophers;

    for (int i = 0; i < n; ++i)
    {
        philosophers.emplace_back(philosopher, i, n, chopsticks.data());
    }

    for (auto &t : philosophers)
    {
        t.join();
    }

    return 0;
}