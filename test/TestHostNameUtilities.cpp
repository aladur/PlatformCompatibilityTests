#include "gtest/gtest.h"
#include "HostNameUtilities.h"
#include "Utilities.h"
#include <string>

#ifndef _WIN32
TEST(TestHostNameUtilities, HostName)
{
    auto name = GetHostName();
    EXPECT_FALSE(name.empty());
}
#endif

TEST(TestHostNameUtilities, DomainName)
{
    EXPECT_NO_THROW(GetDomainName());
}

#ifndef _WIN32
TEST(TestHostNameUtilities, FullyQualifiedDomainName)
{
    auto name = GetFullyQualifiedDomainName();
    EXPECT_FALSE(name.empty());
}

TEST(TestHostNameUtilities, HostNameCLI)
{
    auto name = GetHostNameCLI();
    EXPECT_FALSE(name.empty());
}
#endif

TEST(TestHostNameUtilities, DomainNameCLI)
{
    EXPECT_NO_THROW(GetDomainNameCLI());
}

#ifndef _WIN32
TEST(TestHostNameUtilities, FullyQualifiedDomainNameCLI)
{
    auto name = GetFullyQualifiedDomainNameCLI();
    EXPECT_FALSE(name.empty());
}

TEST(TestHostNameUtilities, CrossCheck)
{
    auto name = ToUpper(GetHostName());
    auto nameCLI = ToUpper(GetHostNameCLI());
    EXPECT_EQ(name, nameCLI);

    name = ToUpper(GetDomainName());
    nameCLI = ToUpper(GetDomainNameCLI());
    //EXPECT_EQ(name, nameCLI);

    name = ToUpper(GetFullyQualifiedDomainName());
    nameCLI = ToUpper(GetFullyQualifiedDomainNameCLI());
    EXPECT_EQ(name, nameCLI);
}
#endif

