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




