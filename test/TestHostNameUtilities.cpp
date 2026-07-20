#include "gtest/gtest.h"
#include "HostNameUtilities.h"
#include "Utilities.h"
#include <string>

TEST(TestHostNameUtilities, HostName)
{
    auto name = GetHostName();
    EXPECT_FALSE(name.empty());
}

TEST(TestHostNameUtilities, DomainName)
{
    EXPECT_NO_THROW(GetDomainName());
}

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

TEST(TestHostNameUtilities, DomainNameCLI)
{
    EXPECT_NO_THROW(GetDomainNameCLI());
}

TEST(TestHostNameUtilities, FullyQualifiedDomainNameCLI)
{
    auto name = GetFullyQualifiedDomainNameCLI();
    EXPECT_FALSE(name.empty());
}

TEST(TestHostNameUtilities, CrossCheck)
{
    auto name = ToLower(GetHostName());
    auto nameCLI = ToLower(GetHostNameCLI());
    EXPECT_EQ(name, nameCLI);

    name = ToLower(GetDomainName());
    nameCLI = ToLower(GetDomainNameCLI());
    //EXPECT_EQ(name, nameCLI);

    name = ToLower(GetFullyQualifiedDomainName());
    nameCLI = ToLower(GetFullyQualifiedDomainNameCLI());
    EXPECT_EQ(name, nameCLI);
}

