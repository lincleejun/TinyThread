#pragma once
#include <iostream>
#include "ThreadBase.h"
#include "utils.h"
using namespace std;

class CTestThread : public IThreadBase
{
public:
    CTestThread()
        :IThreadBase(_T("Test Thread"))
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

class CSimpleMutex
{
public:
    CSimpleMutex()
    {
        mutex_ = CreateMutex(NULL, FALSE, NULL);
    }
    ~CSimpleMutex()
    {
        CloseHandle(mutex_);
    }
    void Lock()
    {
        WaitForSingleObject(mutex_, INFINITE);
    }
    void UnLock()
    {
        ReleaseMutex(mutex_);
    }

private:
    HANDLE mutex_;
};

class CTestLockTime : public IThreadBase
{
public:
    CTestLockTime()
        :IThreadBase(_T("TestLockTimeThread"))
    {

    }
    virtual int Run()
    {
        CSimpleLock lock_;
        DWORD beg = GetTickCount();
        for (int i=0; i<1000000; ++i)
        {
            lock_.Lock();
            lock_.UnLock();
        }
        cout << "critical section total time : " << GetTickCount() - beg << endl;

        CSimpleMutex mutex_;
        beg = GetTickCount();
        for (int i=0; i<1000000; ++i)
        {
            mutex_.Lock();
            mutex_.UnLock();
        }
        cout << "mutex total time : " << GetTickCount() - beg << endl;

        CWaitableEvent event(false, true);
        beg = GetTickCount();
        for (int i=0; i<1000000; ++i)
        {
            event.Wait();
            event.Signal();
        }
        cout << "event total time : " << GetTickCount() - beg << endl;
        return 0;
    }
};
