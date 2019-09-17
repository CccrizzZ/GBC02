#include <iostream>
#include <assert.h>
using namespace std;


int factorial(int x)
{
  assert(x >= 0);
  if (x == 0)
  {
    return 1;
  }

  return (factorial(x - 1) * x);
}


int doubleFactorial(int x)
{
  assert(x >= 0);
  if (x == 0) {
    return 1;
  }

  return (doubleFactorial(x - 2) * x);
}
