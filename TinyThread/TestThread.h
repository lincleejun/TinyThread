#pragma once
#include <iostream>
#include "ThreadBase.h"
using namespace std;

class CTestThread : public ThreadBase
{
public:
    CTestThread()
        :ThreadBase(_T("Test Thread"))
    {

    }
    virtual int Run()
    {
        wcout << GetThreadName() << endl;
        cout << GetThreadID() << endl;
        return 0;
    }
private:

};
