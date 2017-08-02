#define LP3_CORE_API_CREATE
#include "ErrOut.hpp"
#include <lp3/le.hpp>
#ifdef LP3_COMPILE_TARGET_WINDOWS

#include <process.h>
#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <limits.h>

// FU Windows
#undef max

#include <lp3/core.hpp>

// Disable the ever-present error message about strncpy being obsolete.
#pragma warning(disable : 4996)


using lp3::core::WCharToCharConverter;
using std::string;

#define DEFAULT_PIPENAME _TEXT("\\\\.\\pipe\\LpErrOut")

/* Internal errout functions to call a pipe and output data there. */
#define BUFSIZE 512
#define EWBUFFER_SIZE 2048

/* 20 seconds. */
#define HOW_LONG_TO_WAIT_FOR_BUSY_PIPE (20 * 1000)

namespace {

    /* Begin ErrOut functionality.  Should happen
     * at beginning of program if in use. */
    static bool usePipe = false;

    static CRITICAL_SECTION cs;

    static char newLineChar[4] = "^%^";//"\r\n";
    static char ewbuffer[EWBUFFER_SIZE];
    static char syncChar = 0;

    /* So we can print nullptr. */
    /* { <- This is put here to work around a Macaroni bug. ;_'; ... */
    static const char * nullptrString = "}<nullptr>{"; /* } */

    static HANDLE hPipe;

    /* Connects to the pipe server. */
    bool openPipe(TCHAR * pipeName) {
        /* Keep trying to open pipe. */
        hPipe = INVALID_HANDLE_VALUE;
        while(INVALID_HANDLE_VALUE == hPipe)
        {
            hPipe = CreateFile(
                pipeName, /* Name of pipe. */
                GENERIC_WRITE,     /* Access mode. */
                0,                 /* No sharing. */
                nullptr,              /* Default security attributes. */
                OPEN_EXISTING,     /* Opens existing pipe. */
                FILE_FLAG_WRITE_THROUGH,                 /* Default attributes. */ //0?
                nullptr);             /* Not a template file. */
            /* Quit trying to open the pipe if the handle is good. */
            if (INVALID_HANDLE_VALUE == hPipe)
            {
                /* If we get any other error quit trying to open,
                 * otherwise wait a bit for it to open then quit. */
                if (ERROR_PIPE_BUSY != GetLastError() ||
                    !WaitNamedPipe((LPCTSTR)pipeName, HOW_LONG_TO_WAIT_FOR_BUSY_PIPE))
                {
                    return false;
                }
            }
        }
        // The pipe connected; change to message-read mode, fail if can't.
        DWORD dwMode = PIPE_READMODE_BYTE; /*MESSAGE; May need to | with PIPE_NOWAIT. */
        if (!SetNamedPipeHandleState(hPipe,    /* Pipe handle. */
                                     &dwMode,  /* New pipe mode.*/
                                     nullptr,     /* Don't set maximum bytes. */
                                     nullptr))    /* Don't set maximum time. */
        {
            return false;
        }
        return true;
    }

    /* Close pipe. */
    void closePipe()
    {
        CloseHandle(hPipe);
    }

    /* Write message to the pipe. */
    bool writePipe(const char * message)
    {
        DWORD cbWritten;
        if (!WriteFile(hPipe,                   /* Pipe handle */
                       (LPCVOID) message,       /* Message. */
                       (DWORD) (strlen(message)+1) * sizeof(char), /* Message length. */
                       &cbWritten,             /* Bytes written. */
                       nullptr))               /* Not overlapped. */
        {
            return false;
        }
        return true;
    }

}


bool ErrOutPipe::Init()
{
    InitializeCriticalSection(&cs);
    if (!openPipe(DEFAULT_PIPENAME))
    {
        WriteLine("Could not connect to LpErrOut pipe.");
        return false;
    }
    else
    {
        usePipe = true;
        return true;
    }
}

void ErrOutPipe::AssertFunc(bool condition, char const *  file, int line, char const *  msg)
{
    if (!condition)
    {
        Write("Assert occured at ");
        Write(file);
        Write(", line ");
        WriteNumber(line);
        Write(":");
        WriteLineP(0, msg);
    }
}

void ErrOutPipe::Close()
{
    if (usePipe)
    {
        closePipe();
    }
}

void ErrOutPipe::Set(char const *  name, char const *  value)
{

    EnterCriticalSection(&cs);
    std::size_t n, v;
    n = strlen(name);
    v = strlen(value);
    char * newStr = new char[n + v + 2];
    strncpy(newStr, name, n);
    newStr[n] = '@';
    strncpy(newStr + n + 1, value, v);
    newStr[n + v + 1] = '\0';
    Write(newStr);
delete [] newStr;
    LeaveCriticalSection(&cs);
}

void ErrOutPipe::Set(char const *  name, int value)
{
    EnterCriticalSection(&cs);
    char buf[32];
    buf[31] = '\0';
    bool negative = value < 0;
    if (negative)
    {
        value = value * -1;
    }

    int i;
    for (i = 30; i >= 0 && value > 0; i --)
    {
        buf[i] = (char)((value % 10) + 48);
        value /= 10;
    }
    if (i >= 0)
    {
        if (negative)
        {
            buf[i] = '-';
        }
        else
        {
            buf[i] = ' ';
        }
        i --;
    }

    buf[31] = '\0';
    Set(name, buf + i + 1);
    LeaveCriticalSection(&cs);

}

void ErrOutPipe::Trace(char const *  msg)
{
    Set("TRACE", msg);
}

void ErrOutPipe::Write(char const *  msg)
{
        WriteP(127, msg);
}

void ErrOutPipe::WriteLastError(int priority)
{
    // Retrieve the system error message for the last-error code
    LPVOID lpMsgBuf;
    //Unused? --> LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    // Display the error message and exit the process

    //lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
    //    (lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)lpszFunction)+40)*sizeof(TCHAR));
    /*StringCchPrintf((LPTSTR)lpDisplayBuf,
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"),
        lpszFunction, dw, lpMsgBuf);
    */
    //MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);
    WriteLineP(priority, (const char *) lpMsgBuf);//lpDisplayBuf);

    LocalFree(lpMsgBuf);
    //LocalFree(lpDisplayBuf);
}

void ErrOutPipe::WriteLine(char const *  msg)
{
    WriteLineP(127, msg);
}

void ErrOutPipe::WriteLine(wchar_t const *  msg)
{
    WCharToCharConverter converter(msg);
    WriteLineP(127, converter.GetCharPtr());
}

void ErrOutPipe::WriteLine(std::string const  & msg)
{
    using lp3::core::WCharToCharConverter;
    WriteLine(msg.c_str());
}

void ErrOutPipe::WriteLineP(int priority, char const *  msg)
{
    WriteN(priority, msg, true);
}

void ErrOutPipe::WriteLineP(int priority, wchar_t const *  msg)
{
    WCharToCharConverter converter(msg);
    WriteLineP(priority, converter.GetCharPtr());
}

void ErrOutPipe::WriteLinePL(char const *  fileName, int const  lineNumber, int priority, char const *  msg)
{
    using lp3::core::WCharToCharConverter;
    WriteP(priority, fileName);
    WriteP(priority, " ");
    WriteNumberP(priority, lineNumber);
    WriteP(priority, " : ");
    WriteLineP(priority, msg);
}

void ErrOutPipe::WriteLinePL(char const *  fileName, int const  lineNumber, int priority, wchar_t const *  msg)
{
    WCharToCharConverter converter(msg);
    WriteLinePL(fileName, lineNumber, priority, converter.GetCharPtr());
}

void ErrOutPipe::WriteN(int priority, char const *  msg, bool newLine)
{
	(void)priority; // Silence warning.

    using lp3::core::WCharToCharConverter;

        EnterCriticalSection(&cs);
        if (msg == nullptr)
        {
            msg = nullptrString;
        }
        if (usePipe){
           syncChar ++;
           ewbuffer[0] = syncChar; // Put syncChar at beginning...
           std::size_t bufEnd = strlen(msg) + 1;
           if (newLine)
           {
               bufEnd = (bufEnd < EWBUFFER_SIZE - 10) ? bufEnd : EWBUFFER_SIZE - 10;
               ewbuffer[bufEnd] = '^';  // NewLine...
               ewbuffer[bufEnd + 1] = '%';
               ewbuffer[bufEnd + 2] = '^';
               ewbuffer[bufEnd + 3] = syncChar; //And at end.
               ewbuffer[bufEnd + 4] = '\0';
           }
           else
           {
               bufEnd = (bufEnd < EWBUFFER_SIZE - 2) ? bufEnd : EWBUFFER_SIZE - 2;
               ewbuffer[bufEnd] = syncChar;
               ewbuffer[bufEnd + 1] = '\0';
           }
           // Copy the string into the ewBuffer
           strncpy(ewbuffer + 1, msg, bufEnd - 1);
            if (newLine)
            {

            }
            if (!writePipe(ewbuffer))
            {
                closePipe();
                usePipe = false;
            }
        }
        LeaveCriticalSection(&cs);
}

void ErrOutPipe::WriteN(int priority, wchar_t const *  msg, bool newLine)
{
    WCharToCharConverter converter(msg);
    WriteN(priority, converter.GetCharPtr(), newLine);
}

void ErrOutPipe::WriteNumber(int number)
{
    using lp3::core::WCharToCharConverter;
    WriteNumberP(255, number);
}

void ErrOutPipe::WriteNumberL(char const *  name, int value)
{
    EnterCriticalSection(&cs);
    char buf[32];
    buf[31] = '\0';
    int i;
    for (i = 30; i >= 0 && value > 0; -- i)
    {
        buf[i] = (char)((value % 10) + 48);
        value /= 10;
    }
    buf[31] = '\0';

    char * nBuf = buf + i + 1;
    std::size_t n, v;
    n = strlen(name);
    v = strlen(nBuf);
    char * newStr = new char[n + v + 2];

    strncpy(newStr, name, n);
    newStr[n] = ':';
    strncpy(newStr + n + 1, nBuf, v);
    newStr[n + v + 1] = '\0';
    WriteLine(newStr);
    delete [] newStr;

    LeaveCriticalSection(&cs);
}

void ErrOutPipe::WriteNumberP(int priority, signed int number)
{
    EnterCriticalSection(&cs);
    if (number == 0)
    {
        WriteP(priority, "0");
    }
    else if (number == INT_MIN ) //see <limits.h>, -2147483648)
    {
        WriteP(priority, "INFINITY (-2147483648)");
    }
    else
    {
        char buf[32];
        buf[31] = '\0';
        bool negative = number < 0;
        if (negative)
        {
            number = number * -1;
        }

        int i;
        for (i = 30; i >= 0 && number > 0; i --)
        {
            buf[i] = (char)((number % 10) + 48);
            number /= 10;
        }
        if (i >= 0)
        {
            if (negative)
            {
                buf[i] = '-';
            }
            else
            {
                buf[i] = ' ';
            }
            i --;
        }

        buf[31] = '\0';

        WriteP(priority, buf + i + 1);//digits + 1);
    }
    LeaveCriticalSection(&cs);
}

void ErrOutPipe::WriteP(int priority, char const *  msg)
{
    WriteN(priority, msg, false);
}

#endif // LP3_COMPILE_TARGET_WINDOWS
