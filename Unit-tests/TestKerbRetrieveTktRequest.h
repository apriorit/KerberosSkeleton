#ifndef TEST_KERB_RETRIEVE_TKT_REQUEST
#define TEST_KERB_RETRIEVE_TKT_REQUEST

#include "gtest/gtest.h"
#include "KerbRetrieveTktRequest.h"

class TestKerbRetrieveTktRequest : public testing::Test
{
protected:
    typedef std::unique_ptr<KerberosClient::KerbRetrieveTktRequest> KerbRetrieveTktRequestPtr;

    KerbRetrieveTktRequestPtr m_kerbRetrieveTktRequest;
};

struct KerbRetrieveTktRequest_state
{
    std::wstring initial_targetName;
    KERB_PROTOCOL_MESSAGE_TYPE final_MessageType;
    LUID final_LogonId;
    UNICODE_STRING final_TargetName;
    ULONG final_TicketFlags;
    ULONG final_CacheOptions;
    LONG final_EncryptionType;
    SecHandle final_CredentialsHandle;

    friend std::ostream& operator<<(std::ostream& os, const KerbRetrieveTktRequest_state& obj)
    {
        wchar_t* targetName = (wchar_t*)obj.initial_targetName.c_str();
        return os
            << "initial_targetName: " << targetName << std::endl
            << "final_MessageType: " << obj.final_MessageType << std::endl
            << "final_LogonId.LowPart: " << obj.final_LogonId.LowPart << std::endl
            << "final_LogonId.HighPart: " << obj.final_LogonId.HighPart << std::endl
            << "final_TargetName.Length: " << obj.final_TargetName.Length << std::endl
            << "final_TargetName.MaximumLength: " << obj.final_TargetName.MaximumLength << std::endl
            << "final_TargetName.Buffer: " << obj.final_TargetName.Buffer << std::endl
            << "final_TicketFlags: " << std::hex << "0x" << obj.final_TicketFlags << std::endl
            << "final_CacheOptions: " << obj.final_CacheOptions << std::endl
            << "final_EncryptionType: " << obj.final_EncryptionType << std::endl
            << "final_CredentialsHandle.dwLower: " << obj.final_CredentialsHandle.dwLower << std::endl
            << "final_CredentialsHandle.dwUpper: " << obj.final_CredentialsHandle.dwUpper << std::endl;
    }
};

class TestKerbRetrieveTktRequestWithInit : public TestKerbRetrieveTktRequest, public testing::WithParamInterface<KerbRetrieveTktRequest_state>
{
public:
    TestKerbRetrieveTktRequestWithInit();
};

#endif // TEST_KERB_RETRIEVE_TKT_REQUEST

