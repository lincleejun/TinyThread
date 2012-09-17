#pragma once
#include <Windows.h>
#include <tchar.h>
#include "utils.h"
#include "lock.h"

enum LOG_LEVEL
{
    LOG_WARNING,
    LOG_DEBUG,
    LOG_INFO,
    LOG_ERROR,
    LOG_FATAL,
};

#define LOG_FILE_NAME   _T("log.txt")
#define MAX_LOG_LENGTH  128

#define LOG(ll, fmt, ...) LogToFile(ll, fmt, ##__VA_ARGS__)

void LogToFile(LOG_LEVEL ll, const TCHAR* fmt, ...);

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


