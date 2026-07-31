/*
MIT License

Copyright (c) 2026  Wolfgang Schwotzer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "HostNameUtilities.h"
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

#ifndef HOST_NAME_MAX
    #define HOST_NAME_MAX 255
#endif

std::string GetHostName()
{
#ifdef _WIN32
    char buffer[HOST_NAME_MAX + 1]{};
    
    if (gethostname(buffer, sizeof(buffer)) == 0)
    {
        return buffer;
    }
#endif
    
    return {};
}

std::string GetDomainName()
{
#ifdef _WIN32
    FIXED_INFO* pFixedInfo = nullptr;
    ULONG ulOutBufLen = 0;
    
    // First call to get required buffer size
    if (GetNetworkParams(nullptr, &ulOutBufLen) != ERROR_BUFFER_OVERFLOW)
    {
        return {};
    }
    
    pFixedInfo = static_cast<FIXED_INFO*>(malloc(ulOutBufLen));
    if (pFixedInfo == nullptr)
    {
        return {};
    }
    
    if (GetNetworkParams(pFixedInfo, &ulOutBufLen) != NO_ERROR)
    {
        free(pFixedInfo);
        return {};
    }
    
    std::string domainName = pFixedInfo->DomainName;
    free(pFixedInfo);
    
    if (!domainName.empty() && domainName != "(none)")
    {
        return domainName;
    }
#endif
    
    return {};
}

std::string GetFullyQualifiedDomainName()
{
#ifdef _WIN32
    auto hostname = GetHostName();
    struct addrinfo hints{};
    struct addrinfo* info = nullptr;
    
    if (hostname.empty())
    {
        return {};
    }
    
    hints.ai_flags = AI_CANONNAME;
    hints.ai_protocol = 0;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    // Initialize Winsock if not already done
    WSADATA wsaData;
    int wsaInitResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    
    int status = getaddrinfo(hostname.c_str(), nullptr, &hints, &info);
    
    if (wsaInitResult == 0)
    {
        WSACleanup();
    }
    
    if (status != 0)
    {
        return hostname;
    }
    
    std::string result = info->ai_canonname ? info->ai_canonname : hostname;
    freeaddrinfo(info);
    
    return result;
#endif
    
    return {};
}

std::string ExecuteCommand(const char* command)
{
#ifdef _WIN32
    std::string result;
    FILE* fp = _popen(command, "r");
    
    if (fp != nullptr)
    {
        char buffer[256];
        while (true)
        {
            const auto bytesRead = fread(buffer, 1, sizeof(buffer), fp);
            if (bytesRead == 0U)
            {
                break;
            }
            result.append(buffer, bytesRead);
        }
        _pclose(fp);
        
        // Remove trailing newline and carriage return
        size_t pos = result.find_first_of("\r\n");
        if (pos != std::string::npos)
        {
            result.resize(pos);
        }
    }
#endif
    
    return result;
}

std::string GetHostNameCLI()
{
#ifdef _WIN32
    return ExecuteCommand("hostname 2>nul");
#endif
    
    return {};
}

std::string GetDomainNameCLI()
{
#ifdef _WIN32
    return ExecuteCommand("wmic computersystem get domain /value");
#endif
    
    return {};
}

std::string GetFullyQualifiedDomainNameCLI()
{
#ifdef _WIN32
    return ExecuteCommand("hostname 2>nul");
#endif
    
    return {};
}