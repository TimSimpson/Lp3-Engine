#pragma once

#include <string>

inline namespace lp3_core_internal {

// ----------------------------------------------------------------------------
// class ErrOutPipe
// ----------------------------------------------------------------------------
//     Ancient, ancient code which talks to a named pipe.
// ----------------------------------------------------------------------------
class
ErrOutPipe
{
public:
    static bool Init();

    static void AssertFunc(bool condition, char const *  file, int line, char const *  msg);
    static void Close();
    static void Set(char const *  name, char const *  value);
    static void Set(char const *  name, int value);
    static void Trace(char const *  msg);
    static void Write(char const *  msg);
    static void WriteLastError(int priority);
    static void WriteLine(char const *  msg);
    static void WriteLine(wchar_t const *  msg);
    static void WriteLine(std::string const  & msg);
    static void WriteLineP(int priority, char const *  msg);
    static void WriteLineP(int priority, wchar_t const *  msg);
    static void WriteLinePL(char const *  fileName, int const  lineNumber, int priority, char const *  msg);
    static void WriteLinePL(char const *  fileName, int const  lineNumber, int priority, wchar_t const *  msg);
    static void WriteN(int priority, char const *  msg, bool newLine);
    static void WriteN(int priority, wchar_t const *  msg, bool newLine);
    static void WriteNumber(int number);
    static void WriteNumberL(char const *  name, int value);
    static void WriteNumberP(int priority, signed int number);
    static void WriteP(int priority, char const *  msg);
};

}
