#include <iostream>
#include "TemplateExample.cpp"
#include "MemoryAccess.cpp"
using namespace std;


int main(int args, char** argc) {
  cout << "Hello world" << '\n';

  // template function and class
  cout << "Min = " << min_func(32, 54) << '\n';
  cout << "Max = " << max_func(49.3, 38.39) << '\n';


  // calculate the time elapsed
  // set by row
  clock_t begin = clock();
  row_ordered();
  clock_t end = clock();

  cout << "Time Elapsed (row): " << diffclock(begin, end) << "ms" << '\n';

  // set by column
  clock_t begin1 = clock();
  column_ordered();
  clock_t end1 = clock();

  cout << "Time Elapsed (column): " << diffclock(begin1, end1) << "ms" << '\n';




  system("pause");
  return 0;
}
