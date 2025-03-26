// Katy
// giving each process a turn at the shared memory
// #include "petersons_algorithm.cpp"

// just in case- katy
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
    int processCount = 0;

public:
    fine(int n)
    { // default constructor
        processCount = n;
        cout << processCount;
    }
};