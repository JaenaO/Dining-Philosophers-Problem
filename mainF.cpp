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

#include "fine.cpp"
#include "finePeterLock.cpp"

using namespace std;

void philosopher(int id, int n)
{
    finePeterLock chopsticks[n];

    fine a(id);

    while (true)
    {
        a.thinking();

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