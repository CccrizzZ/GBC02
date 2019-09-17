#include <assert.h>
#include <iostream>
using namespace std;

int TriNum(int term){
  // for ( term > 0; term--) {
  //
  // }
  return 0;
}



int TriNumRecursion(int term) {


  // Base case
  if (term == 1) {
    return 1;
  }

  return (TriNumRecursion(term - 1) + term);
}
