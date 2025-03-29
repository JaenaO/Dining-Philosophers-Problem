#include <iostream>
#include <atomic>
#include <thread>
#include <random>

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

  atomic<int> n = atoi(argv[1]);
  thread *philosophers = new thread[n]; // create an array of threads

  for (int i = 0; i < n; i++)
  {
    philosophers[i] = thread(i); // create a thread for each philosopher
  }

  // code from websites provided in instructions pdf
  random_device rd;                           // a seed source for the random number engine
  mt19937 gen(rd());                          // mersenne_twister_engine seeded with rd()
  uniform_int_distribution<> distrib(1, 500); // range of random numbers 1-500

  cout << "Coarse Solution" << endl;
  TTLock coarseLock = TTLock(n);
  for (int i = 0; i < n; i++)
  {
    while (true)
    {
      cout << "Philosopher " << i << ": starts thinking." << endl;
      sleep(distrib(gen));
      cout << "Philosopher " << i << ": ends thinking." << endl;
      if (i < 4)
      {
        coarseLock.acquire(i);
        coarseLock.acquire((i + 1) % n);
      }
      else
      {
        coarseLock.acquire((i + 1) % n);
        coarseLock.acquire(i);
      }
      cout << "Philosopher " << i << ": starts eating." << endl;
      sleep(distrib(gen));
      cout << "Philosopher " << i << ": ends eating." << endl;

      coarseLock.release(i);
      coarseLock.release((i + 1) % n);
    }
  }
  /*
  pseudocode from slides
  repeat (forever)
  begin
  Think
  Feeling Hungry
  if (i < 4) then
    acquire chopstick[i];
    acquire chopstick[(i + 1) (mod 5)];
  else
    acquire chopstick[(i + 1) (mod 5)];
    acquire chopstick[i];
  end if
  Eat
  release chopstick[i];
  release chopstick[(i + 1) (mod 5)];
  end repeat
  */

  philosophers[0].join(); // wait for all threads to finish

  return 0;
}