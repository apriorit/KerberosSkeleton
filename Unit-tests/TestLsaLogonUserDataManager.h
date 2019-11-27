#ifndef TEST_LOGON_USER_DATA_MANAGER
#define TEST_LOGON_USER_DATA_MANAGER

#include "gtest/gtest.h"
#include "LsaLogonUserDataManager.h"
#include "ISystemApiWrapper.h"

class TestLsaLogonUserDataManager : public testing::Test
{
protected:
    KerberosServer::LsaLogonUserDataManagerPtr m_lsaLogonUserDataManager;
};

struct LsaLogonUser_state
{
    std::string initial_origin;
    std::string initial_srcModuleIdentifies;
    TicketData initial_serviceTicket;
    TicketData initial_krbtgtTicket;

    KERB_LOGON_SUBMIT_TYPE final_MessageType;
    ULONG final_Flags;
    ULONG final_ServiceTicketLength;
    ULONG final_TicketGrantingTicketLength;
    PUCHAR final_ServiceTicket;
    PUCHAR final_TicketGrantingTicket;
    LSA_STRING final_lsaOrigin;
    TOKEN_SOURCE final_srcContext;
    LUID final_LogonId;
    QUOTA_LIMITS final_quota_limits;

    friend std::ostream& operator<<(std::ostream& os, const LsaLogonUser_state& obj)
    {
        const char* lasOrigin = obj.initial_origin.c_str();
        const char* srcModuleIdentifies = obj.initial_srcModuleIdentifies.c_str();

        return os
            << "initial_origin: " << lasOrigin << std::endl
            << "initial_srcModuleIdentifies: " << srcModuleIdentifies << std::endl
            << "final_MessageType: " << obj.final_MessageType << std::endl
            << "final_Flags: " << obj.final_Flags << std::endl
            << "final_ServiceTicketLength: " << obj.final_ServiceTicketLength << std::endl
            << "final_TicketGrantingTicketLength: " << obj.final_TicketGrantingTicketLength << std::endl
            << "final_ServiceTicket: " << obj.final_ServiceTicket << std::endl
            << "final_TicketGrantingTicket: " << obj.final_TicketGrantingTicket << std::endl
            << "final_lsaOrigin.Length: " << obj.final_lsaOrigin.Length << std::endl
            << "final_lsaOrigin.MaximumLength: " << obj.final_lsaOrigin.MaximumLength << std::endl
            << "final_lsaOrigin.Buffer: " << obj.final_lsaOrigin.Buffer << std::endl
            << "final_srcContext.SourceName: " << obj.final_srcContext.SourceName << std::endl
            << "final_srcContext..SourceIdentifier.HighPart: " << obj.final_srcContext.SourceIdentifier.HighPart << std::endl
            << "final_srcContext..SourceIdentifier.LowPart: " << obj.final_srcContext.SourceIdentifier.LowPart << std::endl
            << "final_LogonId.LowPart: " << obj.final_LogonId.LowPart << std::endl
            << "final_LogonId.HighPart: " << obj.final_LogonId.HighPart << std::endl
            << "final_quota_limits.MaximumWorkingSetSize: " << obj.final_quota_limits.MaximumWorkingSetSize << std::endl
            << "final_quota_limits.MinimumWorkingSetSize: " << obj.final_quota_limits.MinimumWorkingSetSize << std::endl
            << "final_quota_limits.NonPagedPoolLimit: " << obj.final_quota_limits.NonPagedPoolLimit << std::endl
            << "final_quota_limits.PagedPoolLimit: " << obj.final_quota_limits.PagedPoolLimit << std::endl
            << "final_quota_limits.PagefileLimit: " << obj.final_quota_limits.PagefileLimit << std::endl
            << "final_quota_limits.TimeLimit.HighPart: " << obj.final_quota_limits.TimeLimit.HighPart << std::endl
            << "final_quota_limits.TimeLimit.LowPart: " << obj.final_quota_limits.TimeLimit.LowPart << std::endl;
    }
};

class TestLsaLogonUserDataManagerWithInit : public TestLsaLogonUserDataManager, public testing::WithParamInterface<LsaLogonUser_state>
{
public:
    TestLsaLogonUserDataManagerWithInit();
};

#endif // TEST_KERB_RETRIEVE_TKT_REQUEST
