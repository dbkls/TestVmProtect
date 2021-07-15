// This project should not have any CRT libraries to minimize its size
//

#define _NO_CRT_STDIO_INLINE

#include "framework.h"
#include "TestVmProtect.h"
#include <math.h>
#include <strsafe.h>



#ifdef _DEBUG
#error Build-for-Release-only
#endif

#define FULL_TIMING_TEST            //Uncomment this line to run a full timing test



int __declspec(noinline) ShowMsgBoxFunc(UINT value1, LPCTSTR pString1);
int __declspec(noinline) TimingFunc();

extern "C" {
    void __stdcall breakpoint_func();
}


void MainEP()
{
    int nExitCode = 0;

    ::GdiFlush();           //Added it here to find this chunk of code with a binary debugger


#ifndef FULL_TIMING_TEST
    //Just a simple message box test

    static volatile LPCTSTR gkStr = L"Hello world, obfuscated!\n\nDo you want to trigger a breakpoint?";
    static volatile UINT gnMbBtns = MB_ICONINFORMATION | MB_YESNOCANCEL;

    nExitCode = ShowMsgBoxFunc(gnMbBtns, gkStr);
    if (nExitCode == IDYES)
    {
        breakpoint_func();
    }

#else
    //Full timing test - shows how long it took to execute each time in ms
    WCHAR buff[512] = {};

    for (int i = 0; i < 10; i++)
    {
        DWORD dwBefore = GetTickCount();
        TimingFunc();

        DWORD dwElapsed = GetTickCount() - dwBefore;

        WCHAR buffTemp[32];
        swprintf_s(buffTemp, _countof(buffTemp), L"%u ms\n", dwElapsed);
        StringCchCat(buff, _countof(buff), buffTemp);
    }

    StringCchCat(buff, _countof(buff), L"\nDo you want to trigger a breakpoint?");

    if (::MessageBox(::GetDesktopWindow(), buff, L"Test Obfuscation Timing", MB_ICONINFORMATION | MB_YESNOCANCEL) == IDYES)
    {
        breakpoint_func();
    }

    ::GdiFlush();           //Added it here to find this chunk of code with a binary debugger

#endif




    //Terminate our process (since we don't have CRT)
    ::ExitProcess(nExitCode);
}


int __declspec(noinline) ShowMsgBoxFunc(UINT value1, LPCTSTR pString1)
{
    //Simply show a message box (the easiest of tests)
    return ::MessageBox(::GetDesktopWindow(), pString1, L"Test Obfuscation (Simple)", value1);
}



int __declspec(noinline) TimingFunc()
{
    //Perform some nonsensical methematical operations to kill CPU cycles
    double res = 1.0;

    for (int i = 1000000; i >= 0; i--)
    {
        FILETIME ft;
        GetSystemTimeAsFileTime(&ft);
        if (ft.dwLowDateTime & 1)
        {
            res = i * sin(res / 123.4583 + (i % 10));
        }
        else
        {
            res = (i + 1) * sqrt(res) / (12.156 + (i % 13));
        }
    }

    return (int)(res);
}

