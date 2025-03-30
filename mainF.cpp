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

using namespace std;

void solution()
{
    cout << "solution";
}

int main(int argc, char *argv[])
{
    cout << " \n\n\n---------- Fine Solution ---------\n\n\n";
    if (argc < 2 || atoi(argv[1]) < 2)
    {
        cout << "Usage: " << argv[0] << " <n philosophers (at least 2)>" << endl;
        return 1;
    }

    cout << "Fine Solution\n";
    int n = atoi(argv[1]);

    thread *philosophers = new thread[n]; // create an array of threads

    for (int i = 0; i < n; i++)
    {
        philosophers[i] = thread(i); // create a thread for each philosopher
    }

    for (int i = 0; i < n; i++)
    {
        if (this_thread::get_id() == philosophers[i].get_id())
        {
            fine a(i);
        }
    }

    for (int i = 0; i < n; i++)
    {
        philosophers[i].join();
    }
    return 0;
}