// --------------------------------------------------------------------
//      Listens to the logging system in Windows
// --------------------------------------------------------------------
#define SDL_MAIN_HANDLED
#include <chrono>
#include <ctime>
#include <iostream>
#include <lp3/log.hpp>
#include <lp3/core.hpp>

#ifndef LP3_COMPILE_TARGET_WINDOWS
    #error "This executable works only with Windows"
#endif

#include "windows.h"

namespace {
    struct PipeCloser {
        HANDLE pipe;
        PipeCloser(HANDLE pipe)
            : pipe(pipe)
        {}

        ~PipeCloser() {
            CloseHandle(pipe);
        }
    };

    const int BUFFERSIZE = 4320;

    int listen_to_pipe() {
        const auto security_descriptor
            = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
        if (nullptr == security_descriptor) {
            std::cerr << "error calling local alloc: " << GetLastError() << std::endl;
            std::abort();
        }
        if (!InitializeSecurityDescriptor(security_descriptor, SECURITY_DESCRIPTOR_REVISION))
        {
            std::cerr << "error initializing security descriptor: " << GetLastError() << std::endl;
            return 1;
        }

        SECURITY_ATTRIBUTES security_attributes;
        security_attributes.nLength = sizeof(SECURITY_ATTRIBUTES);
        security_attributes.bInheritHandle = FALSE;
        security_attributes.lpSecurityDescriptor = security_descriptor;

        const auto PIPE_PATH = "\\\\.\\pipe\\LpErrOut";

        const auto pipe_handle = CreateNamedPipe(
            PIPE_PATH,
            PIPE_ACCESS_INBOUND,
            PIPE_TYPE_BYTE | PIPE_WAIT,
            1,
            BUFFERSIZE,
            BUFFERSIZE,
            NMPWAIT_WAIT_FOREVER,
            nullptr // &security_attributes
        );

        if (pipe_handle == nullptr || pipe_handle == INVALID_HANDLE_VALUE) {
            std::cerr << "Could not create pipe \"" << PIPE_PATH << "\"." << std::endl;
            return 1;
        }

        PipeCloser pipe_closer(pipe_handle);

        std::cout << "Waiting for game to start..." << std::endl;

        BOOL result = ConnectNamedPipe(pipe_handle, NULL);
        if (!result) {
            std::cerr << "Could not establish client connection!" << std::endl;
            return 1;
        }
        
        {
            const char block = (char) 220;            
            const auto now = std::chrono::system_clock::now();
            const auto now_t = std::chrono::system_clock::to_time_t(now);
            const auto time_buf_size = 128;
            char time_buf[time_buf_size];
            ctime_s(time_buf, time_buf_size, &now_t);
            std::cout << "\n" << block << block << block << block
                << "Began to talk to program at " << time_buf 
                << std::endl;
        }

        // TODO: 
        // This remainder business doesn't work, but I don't care enough to fix it.
        char remainder[BUFFERSIZE];
        remainder[0] = '\0';

        DWORD  remainder_len  = 0;
        while (true) {
            char buffer[BUFFERSIZE];

            strncpy_s(buffer, remainder, remainder_len);

            DWORD bytes_read = 0;
            const BOOL success = ReadFile(pipe_handle, buffer + remainder_len, (BUFFERSIZE - 1 - remainder_len) * sizeof(char), &bytes_read, nullptr);
            if (!success) {
                std::cerr << "Reading from pipe failed." << std::endl;
                break;
            }
            if (bytes_read <= 0) {
                std::cerr << "Read back zero bytes." << std::endl;
                break;
            }

            DWORD start = 0;
            for (DWORD i = 0; i < bytes_read; ++i) {
                if (buffer[i] == '\0') {
                    std::string next(buffer + start, i - start);
                    std::cout << next;
                    start = i + 1;
                }
                else if (buffer[i] == '^') {
                    if (i + 4 <= bytes_read) {
                        if (buffer[i + 1] == '%' && buffer[i + 2] == '^' && buffer[i + 3] == '\0') {
                            std::string next(buffer + start, i - start);
                            std::cout << next << std::endl;
                        }
                        auto new_line_code_size = 3;                        
                        if (i + 5 <= bytes_read && buffer[i + 4] == '\0') {
                            new_line_code_size++;
                        }
                        start = i + 4;
                        i += 3;
                    }
                }
            }
            // copy remainder to left over buffer.
            remainder_len = bytes_read - start;
            if (remainder_len > 0) {
                if (remainder_len == BUFFERSIZE) {
                    // just print it out
                    std::string next(buffer, BUFFERSIZE);
                    std::cout << next;
                    remainder_len = 0;
                } else {
                    strncpy_s(remainder, buffer + start, remainder_len);
                }
            }
        }
        
        {
            const char block = (char)223;
            const auto now = std::chrono::system_clock::now();
            const auto now_t = std::chrono::system_clock::to_time_t(now);
            const auto time_buf_size = 128;
            char time_buf[time_buf_size];
            ctime_s(time_buf, time_buf_size, &now_t);
            std::cout << block << block << block << block 
                << "Program finished at " << time_buf 
                << std::endl;
        }
        return 0;
    }
}

int main() {
    while (true) {
        const auto result = listen_to_pipe();
        if (0 != result) {
            return result;
        }
    }
}
