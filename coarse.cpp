// Jaena Orozco

#include <iostream>
#include <thread>
#include <atomic>
#include <random>
#include <unistd.h>
#include <chrono>

using namespace std;

// our peterson's lock implementation
// it has two atomic variables: flags to show intenet to enter the critical section, and turn to show which thread's turn it is
class PetersonLock{
  public:
    atomic<bool> flag[2];
    atomic<int> turn;

    PetersonLock(){ 
      flag[0] = false;
      flag[1] = false;
      turn = 0;
    }

    void lock(int pid){
      flag[pid] = true;
      turn = 1 - pid;

      while(flag[1-pid] && turn != pid) // other wants and their turn
        ;  // spin
    }

    void unlock(int pid){
      flag[pid] = false;
    }
};

// our tournament tree lock implementation
// it has two variables: the number of locks and the array of peterson locks
// the array is how the tree is represented
// it is a 2-ary tree where each node is a peterson lock
class TTLock{  
  public:
    int locks;  // number of locks in the tree
    PetersonLock* nodes;  // create an array of peterson locks

    TTLock(int n){  // constructor
      locks = n - 1;
      nodes = new PetersonLock[locks];
    }

    void acquire(int pid, int index){
      
    }
    
    void release(int pid, int index){

    }

    int leaf_position(int pid){  // maps thread ID to its leaf node in the binary tree
      return locks-(locks / 2)+pid;  // each thread starts at a leaf node
    }

    int getLocks(){
      return locks;
    }
};

// helper function for the pihlosopher functions
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
  cout << "Philosopher " << i << ": starts thinking." << endl;
  this_thread::sleep_for(random_ms()); 
  cout << "Philosopher " << i << ": ends thinking." << endl;
}

// philosopher function where it states when it starts and ends eating
// the time between the start and end is randomized
// the parameter i is the philosopher/thread number
void eating(int i){
  cout << "Philosopher " << i << ": starts eating." << endl;
  this_thread::sleep_for(random_ms()); 
  cout << "Philosopher " << i << ": ends eating." << endl;
}

auto philosopherDine = [](int i, TTLock& lock){
  int n = lock.getLocks()+1;
  thinking(i);
  if(i < n-1){
    lock.acquire(i, -1);  
    lock.acquire((i+1)%n, -1);  
  }
  else{
    lock.acquire((i+1)%n, -1);
    lock.acquire(i, -1);  
  }
  eating(i);
  lock.release(i, -1);
  lock.release((i + 1) % n, -1);
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

  return 0;
}