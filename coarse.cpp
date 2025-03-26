#include <iostream>

using namespace std;

void TTLock(){
  
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