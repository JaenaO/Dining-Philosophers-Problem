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
#include "fine.cpp"
using namespace std;

mutex cout_mutex;

void safe_print(const string &message)
{
    lock_guard<mutex> guard(cout_mutex);
    cout << message << endl;
}

void thinking(int id)
{
    safe_print("Philosopher " + to_string(id) + ": starts thinking");
    this_thread::sleep_for(chrono::milliseconds(500));
    safe_print("Philosopher " + to_string(id) + ": ends thinking");
}

void eating(int id)
{
    safe_print("Philosopher " + to_string(id) + ": starts eating");
    this_thread::sleep_for(chrono::milliseconds(500));
    safe_print("Philosopher " + to_string(id) + ": ends eating");
}

void philosopher(int id, int n)
{
    // initialize n locks
    finePeterLock *chopsticks[n];
    for (int i = 0; i < n; i++)
    {
        chopsticks[i] = new finePeterLock();
    }

    thinking(id);

    // Uses the third attempt algorithm from the slides
    if (id < n)
    {
        chopsticks[id]->lock(id);
        chopsticks[(id + 1) % n]->lock((id + 1) % n);
    }
    else
    {
        chopsticks[id]->lock(id);
        chopsticks[(id + 1) % n]->lock((id + 1) % n);
    }
    // critical section
    eating(id);
    // release
    chopsticks[id]->unlock(id);
    chopsticks[(id + 1) % n]->unlock((id + 1) % n);
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

    vector<thread> philosophers;

    for (int i = 0; i < n; ++i)
    {
        philosophers.emplace_back([i, n]()
                                  { philosopher(i, n); });
    }

    for (auto &t : philosophers)
    {
        t.join();
    }

    return 0;
}