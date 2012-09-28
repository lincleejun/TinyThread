#include <Windows.h>
#include <stdarg.h>
#include <assert.h>
#include "log.h"
#include "utils.h"

#pragma warning(disable:4996)

static const TCHAR* s_logInfo[] = {
    _T("LOG_WARNING"),
    _T("LOG_DEBUG"),
    _T("LOG_INFO"),
    _T("LOG_ERROR"),
    _T("LOG_FATAL"),
};

namespace internal
{
    // log实际写的类，
class CLog
{
public:
    /**
    *	\brief      
    *	\param[in]  log文件名
    */
    CLog(const TCHAR* fileName);
    ~CLog();

    bool Open(const TCHAR* fileName);
    int Write(LOG_LEVEL logLevel, const TCHAR* data, int len);
    void Close();

protected:

private:
    CSimpleLock m_lock;
    HANDLE      m_hFile;
};

}

internal::CLog* Log()
{
    static internal::CLog log(LOG_FILE_NAME);
    return &log;
}

static void GetCurrentDateTime(TCHAR* dateTime)
{
    assert(NULL != dateTime);
    SYSTEMTIME sysTime;
    GetLocalTime(&sysTime);
    _stprintf(dateTime, _T("%04d-%02d-%02d %02d:%02d:%02d"), 
        sysTime.wYear, sysTime.wMonth, sysTime.wDay,
        sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
}

void LogToFile(LOG_LEVEL ll, const TCHAR* fmt, ...)
{
    TCHAR text[MAX_LOG_LENGTH] = {};

    va_list vl;
    va_start( vl, fmt );
    _vstprintf(text, fmt , vl);
    va_end( vl);      
    // 最后的字符串处理
#ifdef _DEBUG
    TCHAR dateTime[MAX_LOG_LENGTH] = {};
    GetCurrentDateTime(dateTime);
    _tprintf(_T("%s [%s]  %s\r\n" ), dateTime, s_logInfo[ll], text);
#endif

    Log()->Write(ll, text, _tcsclen(text));
}

namespace internal 
{
    CLog::CLog(const TCHAR* fileName)
        :m_hFile(INVALID_HANDLE_VALUE)
    {
#ifdef _DEBUG
        if (!Open(fileName))
            assert(0);
#else
        Open(fileName);
#endif
    }

    CLog::~CLog()
    {
        Close();
    }

    bool CLog::Open(const TCHAR* fileName)
    {
        Close();
        m_hFile = CreateFile(fileName, GENERIC_WRITE,
            0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (INVALID_HANDLE_VALUE == m_hFile)
            return false;
        SetFilePointer(m_hFile, 0, 0, FILE_END);
        return true;
    }

    int CLog::Write(LOG_LEVEL logLevel, const TCHAR* data, int len)
    {
        TCHAR text[2*MAX_LOG_LENGTH+10] = {};
        TCHAR dateTime[MAX_LOG_LENGTH] = {};
        // 获取当前时间
        GetCurrentDateTime(dateTime);
        // log的格式, 时间 [等级] log内容
        _stprintf(text, _T("%s [%s] %s\r\n"), dateTime, s_logInfo[logLevel], data);
        if (INVALID_HANDLE_VALUE == m_hFile)
            return -1;
        CSimpleAutoLock autoLock(&m_lock);
        DWORD dw = 0;
        if (!WriteFile(m_hFile, text, _tcsclen(text)*sizeof(TCHAR), &dw, NULL))
            return -2;
        return (int)dw;
    }

    void CLog::Close()
    {
        if (INVALID_HANDLE_VALUE != m_hFile)
        {
            CloseHandle(m_hFile);
            m_hFile = INVALID_HANDLE_VALUE;
        }
    }

}

