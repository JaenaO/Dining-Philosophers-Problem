#include "petersons_algorithm.cpp"
#include <iostream>

using namespace std;

class TTLock{  
  int locks = n - 1;  // number of locks in the tree
  PetersonLock* nodes = new PetersonLock[locks];  // create an array of peterson locks

  void acquire(int pid){
    int index = leaf_position(pid);  // get threads leaf node index
    if(index <= 0){
      nodes[index].lock(pid);
      return;
    }

    int parent = (index - 1) / 2;  // move to parent lock
    nodes[parent].lock(pid);
    acquire(pid);  // move up the tree
  }

  void release(int pid){
    int index = leaf_position(pid);  // get threads leaf node index
    if(index <= 0){
      nodes[index].unlock(pid);
      return;
    }

    int parent = (index - 1) / 2;  // move to parent lock
    nodes[parent].unlock(pid);
    release(pid);  // move up the tree
  }

  int leaf_position(int pid){  // maps thread ID to its leaf node in the binary tree
    return locks + pid - 1;  // each thread starts at a leaf node
  }
}

/* pseudocode
TTLock(threads[])
  locks = threads.length - 1  // Number of locks in the tree
  nodes = array of PetersonLock(locks) // Create an array of Peterson Locks

  acquire(pid)
    index = leaf_position(pid)  // Get threads leaf node index
    if index <= 0
      nodes[index].lock(pid)
      return
    end

    parent = (index - 1) / 2  // Move to parent lock
    nodes[parent].lock(pid)
    acquire(pid)  // Move up the tree
  end

  release(pid)
    index = leaf_position(pid)  // Get threads leaf node index
    if index <= 0
      nodes[index].unlock(pid)
      return
    end
    
    parent = (index - 1) / 2  // Move to parent lock
    nodes[parent].unlock(pid)
    release(pid)  // Move up the tree

  end

  leaf_position(pid) // Maps thread ID to its leaf node in the binary tree
    return locks + pid - 1  // Each thread starts at a leaf node
  end
end
*/