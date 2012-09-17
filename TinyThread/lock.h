#pragma once
#include <Windows.h>

class CLock : public CRITICAL_SECTION
{
public:
    CLock()
    {
        InitializeCriticalSection(this);
    }
    ~CLock()
    {
        DeleteCriticalSection(this);
    }
    void Lock()
    {
        EnterCriticalSection(this);
    }
    void UnLock()
    {
        LeaveCriticalSection(this);
    }
    void TryLock()
    {
        TryEnterCriticalSection(this);
    }
};

class CAutoLock
{
public:
    CAutoLock(CLock& lock)
        :m_lock(lock)
    {
        m_lock.Lock();
    }
    ~CAutoLock()
    {
        m_lock.UnLock();
    }
private:
    CLock&      m_lock;
};