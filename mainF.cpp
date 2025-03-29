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

int main(int argc, char *argv[])
{
    if (argc < 2 || atoi(argv[1]) < 2)
    {
        cout << "Usage: " << argv[0] << " <n philosophers (at least 2)>" << endl;
        return 1;
    }

    int n = atoi(argv[1]);
    thread *philosophers = new thread[n]; // create an array of threads

    for (int i = 0; i < n; i++)
    {
        philosophers[i] = thread(i); // create a thread for each philosopher
    }

    // code from websites provided in instructions pdf
    random_device rd;                           // a seed source for the random number engine
    mt19937 gen(rd());                          // mersenne_twister_engine seeded with rd()
    uniform_int_distribution<> distrib(1, 500); // range of random numbers 1-500

    fine algorithm(n);

    return 0;
}