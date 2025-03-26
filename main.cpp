#include "petersons_algorithm.cpp"
#include "coarse.cpp"
#include "fine.cpp"

#include <iostream>
#include <atomic>

using namespace std;

int main(int argc, char* argv[]) {
  if (argc < 2 || atoi(argv[1]) < 2){
    cout << "Usage: " << argv[0] << " <n philosophers (at least 2)>" << endl;
    return 1;
  }

  atomic n = atoi(argv[1]);

  return 0;
}