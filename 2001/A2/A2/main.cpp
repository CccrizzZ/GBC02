#include <iostream>
#include "PriorityQueue.h"
using namespace std;

int main(int argc,char argv[])
{
    PriorityQueue<int> MyPsnQs(4);

    MyPsnQs.push(100, 1);
    MyPsnQs.push(200, 2);
    MyPsnQs.push(300, 3);
    MyPsnQs.push(400, 4);

    // MyPsnQs.pop();

    int listsize = MyPsnQs.GetSize();
    cout << listsize << endl;
    cout << MyPsnQs.front() << endl;

    // LinkNode<int>* temp = MyPsnQs.Front();

    system("pause");
    return 0;
}