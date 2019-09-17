#include <iostream>
#include "factorial.cpp"
#include "Trinumloop.cpp"
using namespace std;

int main(int argc, char const *argv[]) {
  cout << "9! = " << factorial(9) << '\n';
  // cout << "3!! = " << doubleFactorial(3) << '\n';



  cout << TriNumRecursion(25) << '\n';

  system("pause");
  return 0;
}
