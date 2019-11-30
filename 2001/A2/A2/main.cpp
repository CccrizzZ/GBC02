#include <iostream>
#include "LinkedList.h"
using namespace std;

int main(int argc,char argv[])
{
	LinkList<int> Mylist;

    Mylist.Push(100);
    Mylist.Push(100);
    Mylist.Push(100);
    Mylist.Push(100);

    cout << Mylist.GetSize() << endl;
    system("pause");
    return 0;
}