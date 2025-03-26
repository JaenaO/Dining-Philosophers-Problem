#include "petersons_algorithm.cpp"
#include "coarse.cpp"
#include "fine.cpp"

#include <iostream>
#include <atomic>

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

  atomic n = atoi(argv[1]);

  return 0;
}