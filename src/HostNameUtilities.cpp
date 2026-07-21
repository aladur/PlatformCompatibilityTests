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
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#else
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#endif

#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#endif

#ifndef HOST_NAME_MAX
#ifdef _POSIX_HOST_NAME_MAX
#define HOST_NAME_MAX _POSIX_HOST_NAME_MAX
#else
#define HOST_NAME_MAX 255 // use safe default
#endif
#endif

#ifdef _WIN32
WSADATA wsaData{};
#endif
int wsaInitResult{};

int Initialize()
{
#ifdef _WIN32
    if (!wsaData.wVersion && !wsaData.wHighVersion)
    {
        wsaInitResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    }
#endif

    return wsaInitResult;
}

std::string GetHostName()
{
    char buffer[HOST_NAME_MAX + 1U]{};

    if (!Initialize() && gethostname(buffer, sizeof(buffer)) == 0)
    {
        return buffer;
    }

    return {};
}

std::string GetDomainName()
{
#ifdef _WIN32
    FIXED_INFO* pFixedInfo = nullptr;
    ULONG ulOutBufLen = 0U;

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

    std::string result = pFixedInfo->DomainName;
    free(pFixedInfo);

    if (!result.empty() && result != "(none)")
    {
        return result;
    }
#else
    char buffer[HOST_NAME_MAX + 1U]{};

    if (getdomainname(buffer, sizeof(buffer)) == 0 &&
        std::string(buffer) != "(none)")
    {
        return buffer;
    }
#endif
    return {};
}

std::string GetFullyQualifiedDomainName()
{
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

    int status = getaddrinfo(hostname.c_str(), nullptr, &hints, &info);

    if (status != 0)
    {
        return hostname;
    }

    std::string result = info->ai_canonname ? info->ai_canonname : hostname;
    freeaddrinfo(info);

    return result;
}

std::string ExecuteCommand(const char *command)
{
    std::string result;

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#endif
    FILE *fp = popen(command, "r");

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
        pclose(fp);

        const auto pos = result.find_first_of('\n');
        if (pos != std::string::npos)
        {
            result.resize(pos);
        }
    }

    return result;
}

std::string GetHostNameCLI()
{
#ifdef _WIN32
    const char* command = "hostname 2>nul";
#else
    const char* command = "hostname 2>/dev/null";
#endif

    return ExecuteCommand(command);
}

std::string GetDomainNameCLI()
{
#ifdef _WIN32
    return {};
#else
    return ExecuteCommand("hostname -d 2>/dev/null");
#endif
}

std::string GetFullyQualifiedDomainNameCLI()
{
#ifdef _WIN32
    const char* command = "hostname 2>nul";
#else
    const char* command = "hostname -f 2>/dev/null || hostname 2>/dev/null";
#endif

    return ExecuteCommand(command);
}

