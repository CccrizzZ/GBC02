#include <iostream>
#include "Stack.h"
using namespace std;

int main(int argc, char const *argv[])
{
    Stack <int> A(10);
    A.Push(100);
    A.Push(300);
    A.Push(400);
    A.Push(500);
    A.Push(600);
    A.GetSize();
    cout << A.top() << endl;
    return 0;
}
