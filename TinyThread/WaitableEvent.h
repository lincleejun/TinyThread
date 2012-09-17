#pragma once

//
//
//   File Name : WAITABLEEVENT
//   Date :      2012/09/17 14:03:14
//   Author :    lijun(lijun@zlg.cn)
//   Describle:  wait event, not named.
//
#include <Windows.h>

class CWaitableEvent
{
public:
    CWaitableEvent(bool manual_reset, bool initially_signaled);
    ~CWaitableEvent(void);

    void Wait();
    /**
    *	\brief set signal state     
    */
    void Signal();
    void Reset();
    /**
    *	\brief test the event is signal state or not
    */
    bool IsSignaled();
    HANDLE Release();
    /**
    *	\brief Get the event handle
    */
    HANDLE handle() const { return handle_; }

    // not implement
    static size_t WaitMany(CWaitableEvent** waitables, size_t count);

private:
    HANDLE handle_;
};
