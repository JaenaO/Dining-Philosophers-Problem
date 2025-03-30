// Jaena Orozco

#include <iostream>
#include <thread>
#include <atomic>
#include <random>
#include <unistd.h>
#include <chrono>

using namespace std;

// our peterson's lock implementation
// a lock for synchronization between two threads which we will use in our tournament tree lock
class PetersonLock{
  public:
    atomic<bool> flag[2]; // flag to show intenet to enter the critical section
    atomic<int> turn; // turn to show which thread's turn it is

    // constructor to initalize the variables
    PetersonLock(){ 
      flag[0] = false;
      flag[1] = false;
      turn = 0;
    }

    // lock function for a certain thread, pid, to acquire the lock
    // alogrithm follows the solution given in the slides
    void lock(int pid){ 
      flag[pid] = true;
      turn = 1 - pid;

      while(flag[1-pid] && turn != pid) // other wants and their turn
        ;  // spin
    }

    // unlock function for a certain thread, pid, to release the lock
    void unlock(int pid){
      flag[pid] = false;
    }
};


//our tournament tree lock implementation
//this lock is the coarse solution to the dining philosophers problem
class TTLock{  
  public:
    int n; // number of philosophers
    int locks;  // number of locks in the tree
    PetersonLock* nodes;  // create an array of peterson locks

    // constructor to initialize the number of philosophers, locks, and array
    TTLock(int num){ 
      n = num;
      locks = n-1;
      nodes = new PetersonLock[locks];
    }

    // recursive acquire function to acquire the lock for a certain thread, pid
    // it goes up the tree and acquires locks on the way until it reaches the root where it wins
    // when it becomes the winner of the tournament will be the one to acquire the lock and go into critical section
    void acquire(int pid){
      
    }
    
    // recursive release function to release the lock for a certain thread, pid
    // it goes down the tree and unlocks the locks that were previously acquired by the thread
    void release(int pid){
      
    }

    // function to get the number of philosophers
    int getn(){
      return n;
    }

    // destructor to delete the array of locks/tree nodes
    ~TTLock(){  
      delete[] nodes;
    }
};

// helper function for the philosopher functions below
// it generates a random number between 1 and 500ms for the sleep time
chrono::milliseconds random_ms(){
  // code from websites provided in instructions pdf
  random_device rd;  // a seed source for the random number engine
  mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
  uniform_int_distribution<> distrib(1, 500); // range of random numbers 1-500
  return chrono::milliseconds(distrib(gen)); 
}

// philosopher function where it states when it starts and ends thinking
// the time between the start and end is randomized
// the parameter i is the philosopher/thread number
void thinking(int i){
  chrono::milliseconds rand = random_ms();
  cout << "Philosopher " << i << ": starts thinking." << endl;
  this_thread::sleep_for(rand); 
  cout << "Philosopher " << i << ": ends thinking.\tThinking time: " << rand.count() << "ms" << endl;
}

// philosopher function where it states when it starts and ends eating
// the time between the start and end is randomized
// the parameter i is the philosopher/thread number
void eating(int i){
  chrono::milliseconds rand = random_ms();
  cout << "Philosopher " << i << ": starts eating." << endl;
  this_thread::sleep_for(rand); 
  cout << "Philosopher " << i << ": ends eating.\tEating time: " << rand.count() << "ms" << endl;
}

// the main philosopher function where it calls the thinking and eating functions above
// i is the philosopher/thread number and it uses the TTLock for synchronization
auto philosopherDine = [](int i, TTLock& lock){
  int n = lock.getn();

  thinking(i);
  lock.acquire(i);
  eating(i);
  lock.release(i);
};

int main(int argc, char *argv[])
{
  if (argc < 2 || atoi(argv[1]) < 2)
  {
    cout << "Usage: " << argv[0] << " <n philosophers (at least 2)>" << endl;
    return 1;
  }

  int n = atoi(argv[1]);
  thread* philosophers = new thread[n];  // create an array of threads
  TTLock coarseLock = TTLock(n);

  for(int i=0; i<n; i++){
    philosophers[i] = thread(philosopherDine, i, ref(coarseLock));
  }
  
  for( int i=0; i<n; i++){
    philosophers[i].join();  // wait for all threads to finish
  }

  delete[] philosophers;

  return 0;
}