#include <iostream>
#include "PriorityQueue.h"
using namespace std;

int main(int argc,char argv[])
{
    PriorityQueue<int> MyPsnQs;

    MyPsnQs.Push(100, 1);
    MyPsnQs.Push(200, 2);
    MyPsnQs.Push(300, 3);
    MyPsnQs.Push(400, 4);

    MyPsnQs.Pop();

    int listsize = MyPsnQs.GetSize();
    cout << listsize << endl;
    cout << MyPsnQs.Front() << endl;

    system("pause");
    return 0;
}