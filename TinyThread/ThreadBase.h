//
//
//   File Name : THREADBASE
//   Date :      2012/09/17 14:09:12
//   Author :    lijun(lijun@zlg.cn)
//   Describle:  two method thread base
//               1.one is used inherit run method.
//               2.another is inherit delegate, and add this
//                 object to threadbase. 
//               
// 

#ifndef _THREAD_BASE_H
#define _THREAD_BASE_H

#include "NonCopyable.h"
#include "WaitableEvent.h"
#include <string>
#include <tchar.h>
using std::wstring;


/*
 *	thread stack options
 */
class Options
{
public:
    Options(int stackSize = 0, int priority = 0)
        :stackSize_(stackSize), priority_(priority)
    {
    }

    int stackSize_;
    int priority_;
};
/*
 *	if you want to write a thread, this provide a base implmenet.
 */
class IThreadBase
{
    DECLARE_NON_COPY_ABLE(IThreadBase)
public:
    IThreadBase(const wstring& name = _T(""));
    ~IThreadBase() {}

    bool Start(Options op = Options());
    void Join(int waitTimes = INFINITE);

    virtual int Run() = 0;

    void SetPriority(int priority);

    wstring GetThreadName()
    {
        return threadName_;
    }
    UINT GetThreadID()
    {
        return threadID_;
    }

    friend DWORD WINAPI ThreadProc(LPVOID lpParameter);
private:
    wstring         threadName_;
    UINT            threadID_;
    CWaitableEvent  event_;
    HANDLE          thread_;
};

// runable class
class IDelegate
{
public:
    virtual ~IDelegate() {}
    virtual void Run() = 0;
};

// use runable threadbase
class IThreadBase2 : public IThreadBase
{
    DECLARE_NON_COPY_ABLE(IThreadBase2)
public:
    IThreadBase2(IDelegate* delegate, const wstring& name = _T(""))
        :IThreadBase(name), delegate_(delegate)
    {
    }
    virtual int Run()
    {
        if (NULL != delegate_)
            delegate_->Run();
        return 0;
    }

private:
    IDelegate* delegate_;
};


#endif // _THREAD_BASE_H
