// Katy
// giving each process a turn at the shared memory
// #include "petersons_algorithm.cpp"

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

class fine
{
private:
    int pnum = 0;
    int sleep;
    random_device rd; // a seed source for the random number engine
    mt19937 gen;
    uniform_int_distribution<> distrib;

public:
    fine(int n)
    { // default constructor
        pnum = n;
        sleep = 0;
        cout << pnum << endl;

        mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
        uniform_int_distribution<> distrib(1, 500);
    }

    void randomizeSleep()
    {
        sleep = distrib(gen);
    }

    void eating()
    {
        cout << "Philosopher " << pnum << ": starts eating\n";
        // lock processes

        this_thread::sleep_for(chrono::milliseconds(sleep));
        cout << "Philosopher " << pnum << ": ends eating\n";
    }

    void thinking()
    {
        cout << "Philosopher " << pnum << ": starts thinking\n";
        this_thread::sleep_for(chrono::milliseconds(sleep));
        cout << "Philosopher " << pnum << ": ends thinking\n";
    }
};