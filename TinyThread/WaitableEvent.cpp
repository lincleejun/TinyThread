#include <cassert>
#include "WaitableEvent.h"


CWaitableEvent::CWaitableEvent(bool manual_reset, bool initially_signaled)
{
    handle_ = CreateEvent(NULL, manual_reset, initially_signaled, NULL);
    assert(INVALID_HANDLE_VALUE != handle_);
}

CWaitableEvent::~CWaitableEvent(void)
{
    CloseHandle(handle_);
}

void CWaitableEvent::Wait()
{
    WaitForSingleObject(handle_, INFINITE);
}

void CWaitableEvent::Signal()
{
    SetEvent(handle_);
}

void CWaitableEvent::Reset()
{
    ResetEvent(handle_);
}

bool CWaitableEvent::IsSignaled()
{
    DWORD dw = WaitForSingleObject(handle_, 0);
    if (WAIT_TIMEOUT == dw)
        return false;
    else 
        return true;
}

size_t CWaitableEvent::WaitMany(CWaitableEvent** waitables, size_t count )
{
    // 暂时用不到。。不实现。
    return 0;
}

HANDLE CWaitableEvent::Release()
{
    HANDLE hdl = handle_;
    handle_ = INVALID_HANDLE_VALUE;
    return hdl;
}
