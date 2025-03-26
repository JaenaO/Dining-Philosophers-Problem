/*Your program should take a single command-line argument that specifies the number of philosophers n. 
Note that n need not be a power of two. But you can assume that n is atleast two.*/
#include <iostream>

using namespace std;

void main(int argc, char* argv[])
{
  // Check if the number of arguments is correct
  if(argc != 2)
  {
    cout << "Usage: " << argv[0] << " <number of philosophers>" << endl;
    return;
  }
  
  if (atoi(argv[1]) < 2)
  {
    cout << "Number of philosophers should be at least 2" << endl;
    return;
  }

  int n = atoi(argv[1]);

}