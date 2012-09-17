#include <iostream>
#include "TestThread.h"
#include "IThread.h"
using namespace std;

class Test : public IDelegate
{
public:
    Test(int id)
        :id_(id)
    {

    }
    void Run()
    {
        printf("test : %d\r\n", id_);
    }

    int id_;
};

void TestThreadPool()
{
    IThreadPool pool(10);
    
    Test* test[10] = {NULL};
    for (int i=0; i<10; ++i)
    {
        test[i] = new Test(i+1);
    }

    pool.Start();
    for (int i=0; i<10; ++i)
    {
        pool.AddWork(test[i]);
    }
    
    pool.JoinAll();
    for (int i=0; i<10; ++i)
    {
        delete test[i];
    }
    
}

int main()
{
    cout << "begin test : " << endl;
    CTestThread testThread;
    testThread.Start();
    testThread.Join();

    TestThreadPool();
    cout << "end test :" << endl;
    return 0;
}