#include <iostream>
#include "TestSpnValidator.h"

void TestSpnValidator::SetUp()
{
    spnValidator.reset(new KerberosClient::SpnValidator());
}

void TestSpnValidator::TearDown()
{
    spnValidator.reset(nullptr);
}

#define THIS_TEST(name)\
        TEST_F(TestSpnValidator, name)

THIS_TEST(CheckValidSPN)
{
    ASSERT_TRUE(spnValidator->Validate(L"host/DnsHostname.DnsDomain@DnsDomain"));
}
THIS_TEST(CheckValidSPN_2)
{
    ASSERT_TRUE(spnValidator->Validate(L"host/Dns_Host_name.DnsDomain@DnsDomain"));
}
THIS_TEST(CheckValidSPN_numberInEnd)
{
    ASSERT_TRUE(spnValidator->Validate(L"host/DnsHostname8.DnsDomain@DnsDomain"));
}
THIS_TEST(CheckValidSPN_numberInBeg)
{
    ASSERT_TRUE(spnValidator->Validate(L"host/DnsHostname.DnsDomain@45DnsDomain"));
}
THIS_TEST(CheckValidSPN_dotInDnsDomain)
{
    ASSERT_TRUE(spnValidator->Validate(L"host/dai.dev.local@dev.local"));
}


THIS_TEST(CheckInvalidSPN_hostMarkerAbsent)
{
    ASSERT_FALSE(spnValidator->Validate(L"http/DnsHostname.DnsDomain@DnsDomain"));
}
THIS_TEST(CheckInvalidSPN_SeparatorAbsent)
{
    ASSERT_FALSE(spnValidator->Validate(L"host/DnsHostname.DnsDomain-DnsDomain"));
}
THIS_TEST(CheckInvalidSPN_incorrectDomain)
{
    ASSERT_FALSE(spnValidator->Validate(L"host/DnsHostname.DnsDomain@_DnsDomain"));
}
THIS_TEST(CheckInvalidSPN_incorrectHostName)
{
    ASSERT_FALSE(spnValidator->Validate(L"host/ DnsHostname.DnsDomain@DnsDomain"));
}
THIS_TEST(CheckInvalidSPN_spacesInEnd)
{
    ASSERT_FALSE(spnValidator->Validate(L"host/DnsHostname.DnsDomain@DnsDomain        "));
}
THIS_TEST(CheckInvalidSPN_spacesInBeg)
{
    ASSERT_FALSE(spnValidator->Validate(L"      host/DnsHostname.DnsDomain@DnsDomain"));
}

