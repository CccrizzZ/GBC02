#include <iostream>
#include "PriorityQueue.h"
using namespace std;

// Test Class
class CyberMessage
{
public:
    CyberMessage(int pVal, int dataIn)
    {
        priority = pVal;
        data = dataIn;
    }

    ~CyberMessage()
    {
    }

    int priority;
    int data;
};

int main(int argc,char argv[])
{
    PriorityQueue<int> MyPsnQs(4);

    MyPsnQs.push(100, 1);
    MyPsnQs.push(200, 2);
    MyPsnQs.push(20, 2);
    MyPsnQs.push(100, 1);
    MyPsnQs.push(300, 3);
    MyPsnQs.push(400, 4);
    MyPsnQs.push(300, 3);

    

    int listsize = MyPsnQs.GetSize();
    cout << "List Size: " << listsize << endl;
    


    system("pause");
    return 0;
}