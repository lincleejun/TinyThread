#pragma once
#include <Windows.h>


/**
*	\brief 获取应用程序路径
*/
bool GetAppDir(TCHAR* path);
/**
*\brief  读取平台身份标识码
*/
bool epcGetSystemIdentityCode(ULONGLONG* pAuthorData);

/**
*	\brief  获取GUID
*/
bool GetGuid(TCHAR* guid);
