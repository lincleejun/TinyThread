#pragma once

#include <Windows.h>

/**
*\brief ¿í×Ö·û×ªÎª¶à×Ö½Ú×Ö·û
*/
inline BOOL WC2MB(LPCWSTR lpcwszStr, LPSTR lpszStr, DWORD dwSize = MAX_PATH)
{
    DWORD dwMinSize;
    dwMinSize = WideCharToMultiByte(CP_OEMCP, NULL, lpcwszStr, -1, NULL, 0, NULL, FALSE);
    if(dwSize < dwMinSize)
    {
        return FALSE;
    }
    WideCharToMultiByte(CP_OEMCP, NULL, lpcwszStr, -1, lpszStr, dwSize, NULL, FALSE);

    return TRUE;
}

/**
*\brief ¶à×Ö½Ú×Ö·û×ª¿í×Ö½Ú×Ö·û
*/
inline BOOL MB2WC(LPCSTR lpcszStr, LPWSTR lpwszStr, DWORD dwSize = MAX_PATH)
{
    DWORD dwMinSize;
    dwMinSize = MultiByteToWideChar(CP_ACP, 0, lpcszStr, -1, NULL, 0);
    if(dwSize < dwMinSize)
    {
        return FALSE;
    }
    MultiByteToWideChar(CP_ACP, 0, lpcszStr, -1, lpwszStr, dwMinSize);

    return TRUE;
}