#include <time.h>
#include <iostream>
using namespace std;

const int n = 500;
float TestData[n][n][n];

double diffclock(clock_t clock1, clock_t clock2)
{
  // get ticks
  double diffticks = clock2 - clock1;

  // convert ticks to milliseconds
  double diffms = (diffticks * 10) / CLOCKS_PER_SEC;
  return diffms;
}

// Order a row
void row_ordered(){
  for (int i = 0; i < n; i++){
    for (int j = 0; j < n; j++){
      for (int k = 0; k < n; k++){
        TestData[i][j][k] = 1;
      }
    }
  }
}

// Order a column
void column_ordered() {
  for (int k = 0; k < n; k++) {
    for (int j = 0; j < n; j++) {
      for (int i = 0; i < n; i++) {
        TestData[i][j][k] = 1;
      }
    }
  }
}
