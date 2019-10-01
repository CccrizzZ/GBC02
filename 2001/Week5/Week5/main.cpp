#include <iostream>
#include "LinkList.h"
using namespace std;

int main(int argc, char const *argv[])
{
    cout << "Single Link List Example" << endl;

    LinkList<int> listA;
    listA.Push(100);
    listA.Push(80);
    listA.Push(50);
    listA.Pop();
    cout << listA.GetSize() << endl;
    system("Pause");
    return 0;
}
