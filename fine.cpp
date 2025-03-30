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
#include "finePeterLock.cpp"

class fine
{
private:
    int pnum;
    int sleep;
    random_device rd; // a seed source for the random number engine
    mt19937 gen;
    uniform_int_distribution<> distrib;
    finePeterLock *i;
    finePeterLock *j;

public:
    fine(int n, finePeterLock *A, finePeterLock *B) : pnum(n), sleep(0), gen(rd()), distrib(1, 500), i(A), j(B)
    { // default constructor
        pnum = n;
        sleep = 0;
        mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
        uniform_int_distribution<> distrib(1, 500);
        i = A;
        j = B;
    }

    void randomizeSleep()
    {
        sleep = distrib(gen);
    }

    void thinking()
    {
        cout << "Philosopher " << pnum << ": starts thinking\n";
        randomizeSleep();

        this_thread::sleep_for(chrono::milliseconds(500));

        cout << "Philosopher " << pnum << ": ends thinking\n";

        // philosopher is now hungry
        bool readyToEat = false;
        int iOrj;
        while (readyToEat == false)
        {
            if (i->checkLockStatus() == 0)
            {
                iOrj = 0;
                readyToEat = true;
            }
            else if (j->checkLockStatus() == 0)
            {
                iOrj = 1;
                readyToEat = true;
            }
            // spin until it finds an open lock
        }

        if (iOrj == 0)
        {
            eating(i);
        }
        else
        {
            eating(j);
        }
    }

    void eating(finePeterLock *L)
    {
        cout << "Philosopher " << pnum << ": starts eating\n";
        // lock processes
        L->lock(pnum);
        // nom nom nom
        randomizeSleep();
        this_thread::sleep_for(chrono::milliseconds(sleep));
        L->unlock(pnum);
        cout << "Philosopher " << pnum << ": ends eating\n";
    }

    int getNum()
    {
        return pnum;
    }
};