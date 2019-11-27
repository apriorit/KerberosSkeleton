#include "TestKerbRetrieveTktRequest.h"

using namespace KerberosClient;

static LUID GetLUID()
{
    LUID luidLogonId;
    memset(&luidLogonId, 0, sizeof(LUID));
    return luidLogonId;
}

static UNICODE_STRING GetSpnTargetName()
{
    static std::wstring spnTargetName(L"host/DnsHostname.DnsDomain@DnsDomain");

    UNICODE_STRING ret;
    ret.Length = static_cast<USHORT>(spnTargetName.length() * sizeof(wchar_t));
    ret.MaximumLength = static_cast<USHORT>(spnTargetName.length() * sizeof(wchar_t));
    ret.Buffer = (wchar_t*)(spnTargetName.c_str());

    return ret;
}

static UNICODE_STRING GetKrbtgtTargetName()
{
    static std::wstring spnTargetName(L"krbtgt");

    UNICODE_STRING ret;
    ret.Length = static_cast<USHORT>(spnTargetName.length() * sizeof(wchar_t));
    ret.MaximumLength = static_cast<USHORT>(spnTargetName.length() * sizeof(wchar_t));
    ret.Buffer = (wchar_t*)(spnTargetName.c_str());

    return ret;
}

static SecHandle GetSecHandle()
{
    SecHandle hSec;
    memset(&hSec, 0, sizeof(SecHandle));
    return hSec;
}

TestKerbRetrieveTktRequestWithInit::TestKerbRetrieveTktRequestWithInit()
{
    m_kerbRetrieveTktRequest.reset(new KerbRetrieveTktRequest(GetParam().initial_targetName));
}

TEST_P(TestKerbRetrieveTktRequestWithInit, TicketRequests)
{
    auto expectedResult = GetParam();

    const int ticketFlags = 0x60A00000;
    const int cacheOptions = (GetParam().initial_targetName == L"krbtgt" ? KERB_RETRIEVE_TICKET_AS_KERB_CRED : KERB_RETRIEVE_TICKET_DEFAULT);

    m_kerbRetrieveTktRequest->SetTicketFlags(ticketFlags);
    m_kerbRetrieveTktRequest->SetCacheOptions(cacheOptions);
    KERB_RETRIEVE_TKT_REQUEST* result = m_kerbRetrieveTktRequest->GetRetrieveTktRequest();

    EXPECT_EQ(result->MessageType, expectedResult.final_MessageType);
    EXPECT_EQ(result->LogonId.HighPart, expectedResult.final_LogonId.HighPart);
    EXPECT_EQ(result->LogonId.LowPart, expectedResult.final_LogonId.LowPart);
    EXPECT_EQ(result->TargetName.Length, expectedResult.final_TargetName.Length);
    EXPECT_EQ(result->TargetName.MaximumLength, expectedResult.final_TargetName.MaximumLength);
    EXPECT_EQ(wcscmp(result->TargetName.Buffer, expectedResult.final_TargetName.Buffer), 0);
    EXPECT_EQ(result->TicketFlags, expectedResult.final_TicketFlags);
    EXPECT_EQ(result->CacheOptions, expectedResult.final_CacheOptions);
    EXPECT_EQ(result->EncryptionType, expectedResult.final_EncryptionType);
    EXPECT_EQ(result->CredentialsHandle.dwLower, expectedResult.final_CredentialsHandle.dwLower);
    EXPECT_EQ(result->CredentialsHandle.dwUpper, expectedResult.final_CredentialsHandle.dwUpper);
}

INSTANTIATE_TEST_CASE_P(Default, TestKerbRetrieveTktRequestWithInit,
    testing::Values(
        KerbRetrieveTktRequest_state{ L"host/DnsHostname.DnsDomain@DnsDomain",
                                      KERB_PROTOCOL_MESSAGE_TYPE::KerbRetrieveEncodedTicketMessage,
                                      GetLUID(),
                                      GetSpnTargetName(),
                                      0x60A00000,
                                      0,
                                      0,
                                      GetSecHandle() },
        KerbRetrieveTktRequest_state{ L"krbtgt",
                                      KERB_PROTOCOL_MESSAGE_TYPE::KerbRetrieveEncodedTicketMessage,
                                      GetLUID(),
                                      GetKrbtgtTargetName(),
                                      0x60A00000,
                                      8,
                                      0,
                                      GetSecHandle() }
));