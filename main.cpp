// #include "petersons_algorithm.cpp"
// #include "coarse.cpp"
#include "fine.cpp"

#include <iostream>
#include <atomic>
#include <thread>

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

int main(int argc, char *argv[])
{
  if (argc < 2 || atoi(argv[1]) < 2)
  {
    cout << "Usage: " << argv[0] << " <n philosophers (at least 2)>" << endl;
    return 1;
  }
  int n = atoi(argv[1]);
  atomic n = atoi(argv[1]);
  thread* philosophers = new thread[n];  // create an array of threads

  for(int i=0; i<n; i++){
    philosophers[i] = thread(i);  // create a thread for each philosopher
  }

  cout << "Coarse Solution" << endl;
  TTLock coarseLock = TTLock(philosophers);
  for(int i=0; i<n; i++){
    coarseLock.acquire(i);  // acquire lock for each philosopher
    /*Enter code for eating and thinking*/
    coarseLock.release(i);  // release lock for each philosopher
  }
    
  philosophers[0].join();  // wait for all threads to finish

  cout << "Fine Solution" << endl;

  for(int i=0; i<n; i++){
    philosophers[i].join();  // wait for all threads to finish
  }

  fine algorithm(n);

  return 0;
}