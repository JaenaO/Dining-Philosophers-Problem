// Jaena Orozco

#include <iostream>
#include <thread>
#include <atomic>
#include <random>
#include <unistd.h>
#include <chrono>
#include <cmath>

using namespace std;

// our peterson's lock implementation
// a lock for synchronization between two threads which we will use in our tournament tree lock
class PetersonLock{
  public:
    atomic<bool> flag[2]; // flag to show intenet to enter the critical section
    atomic<int> turn; // turn to show which thread's turn it is

    // constructor to initalize the variables
    PetersonLock(){ 
      flag[0].store(false, memory_order_relaxed);
      flag[1].store(false, memory_order_relaxed);
      turn.store(0, memory_order_relaxed);
      
    }

    // lock function for a certain thread, pid, to acquire the lock
    // alogrithm follows the solution given in the slides
    void lock(int pid){ 
      flag[pid].store(true, memory_order_release);
      turn.store(1-pid, memory_order_release);

      while(flag[1-pid].load(memory_order_acquire) && turn.load(memory_order_acquire) != pid) // other wants and their turn
        ;  // spin
    }

    // unlock function for a certain thread, pid, to release the lock
    void unlock(int pid){
      flag[pid].store(false, memory_order_release);
    }
};

//our tournament tree lock implementation
//this lock is the coarse solution to the dining philosophers problem
class TTLock {
  private:
    int n; // number of philosophers n
    int numLocks; // number of locks
    PetersonLock* locks;  // Tree structure stored as an array
  
  public:
    // constructor to initialize the number of philosophers and locks and locks array
    TTLock(int philosophers){
      n = philosophers;
      numLocks = n-1;
      locks = new PetersonLock[numLocks];
    }
  
    // acquire function to acquire the lock for a certain philosopher pid
    // when it "wins" the tournament, it will go into the critical section
    void acquire(int pid) {
      int start = numLocks+pid;
      int flag = start%2;
      acquire_helper(start, flag);
    }

    // helper function to acquire the locks
    // it will be called by the acquire function and assist in acquiring the locks with recursion
    // the parameters are the current node and the flag index for petersons algorithm
    void acquire_helper(int currentNode, int flag){
      if (currentNode == 0){ // base case: current has reached the root and can go into the critical section
        return;
      }
      else{
        int nextNode = (currentNode-1)/2;
        locks[nextNode].lock(flag);
        
        int nextFlag = nextNode%2;
        acquire_helper(nextNode, nextFlag);
      }
    }
  
    // release function to release the lock for a certain philosopher pid
    // it will go back up the tree and release all the locks it acquired
    void release(int pid) {
      int start = numLocks+pid;
      int flag = start%2;
      release_helper(start, flag);
    }

    // helper function to release the locks
    // it will be called by the release function and assist in releasing the locks with recursion
    // the parameters are the current node and the flag index for petersons algorithm
    void release_helper(int currentNode, int flag){
      if (currentNode == 0){ // base case: current has reached the root so all locks have been released
        return;
      }
      else{
        int nextNode = (currentNode-1)/2;
        locks[nextNode].unlock(flag);

        int nextFlag = nextNode%2;
        release_helper(nextNode, nextFlag);
      } 
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
  cout << "Philosopher " << i << ": ends thinking.\t" << i << " thinking time: " << rand.count() << "ms" << endl;
}

// philosopher function where it states when it starts and ends eating
// the time between the start and end is randomized
// the parameter i is the philosopher/thread number
void eating(int i){
  chrono::milliseconds rand = random_ms();
  cout << "Philosopher " << i << ": starts eating." << endl;
  this_thread::sleep_for(rand); 
  cout << "Philosopher " << i << ": ends eating.\t" << i << " eating time: " << rand.count() << "ms" << endl;
}

// the main philosopher function where it calls the thinking and eating functions above
// i is the philosopher/thread number and it uses the TTLock for synchronization
auto philosopherDine = [](int i, TTLock& lock){
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
  thread* philosophers = new thread[n];  // create an array of threads for n philosophers
  TTLock coarseLock = TTLock(n); 

  for(int i=0; i<n; i++){
    philosophers[i] = thread(philosopherDine, i, ref(coarseLock));
  }
  
  for(int i=0; i<n; i++){
    philosophers[i].join();  // wait for all threads to finish
  }

  return 0;
}