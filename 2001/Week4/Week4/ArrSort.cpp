#include <iostream>
#include "UnorderedArray.h"
#include "OrderedArray.h"

using namespace std;

int main(int argc, char const *argv[])
{
    UnorderedArray<int> myarr(8);
    myarr.Push(0);
    myarr.Push(99);
    myarr.Push(12);
    myarr.Push(6);
    myarr.Push(18);
    myarr.Push(35);
    myarr.Push(60);
    myarr.Push(25);

    cout << "Before sorting" << endl;
    for (int i = 0; i < myarr.getSize(); i++)
    {
        std::cout << myarr[i] << " ";
    }
    cout << endl;

    myarr.MergeSort();

    cout << "After sorting" << endl;
    for (int i = 0; i < myarr.getSize(); i++)
    {
        std::cout << myarr[i] << " ";
    }
    cout << endl;

    system("pause");
    return 0;
}
