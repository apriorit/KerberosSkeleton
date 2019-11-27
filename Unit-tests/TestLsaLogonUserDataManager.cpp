#include "TestLsaLogonUserDataManager.h"
#include "Mock/MockSystemApiWrapper.h"
#include "KerbServerException.h"

using namespace KerberosServer;

static LUID GetLUID()
{
    LUID luidLogonId;
    memset(&luidLogonId, 0, sizeof(LUID));
    return luidLogonId;
}

static QUOTA_LIMITS GetQUOTA_LIMITS()
{
    QUOTA_LIMITS result;
    memset(&result, 0, sizeof(QUOTA_LIMITS));
    return result;
}

static LSA_STRING CreateLsaStringFromString(const std::string& str)
{
    LSA_STRING result;
    result.Length = static_cast<USHORT>(str.size());
    result.MaximumLength = static_cast<USHORT>(str.size());
    result.Buffer = (char*)str.data();

    return result;
}

static TOKEN_SOURCE CreateTokenSourceFromString(const std::string& str)
{
    TOKEN_SOURCE result;
    memset(&result, 0, sizeof(TOKEN_SOURCE));

    strcpy_s(result.SourceName, TOKEN_SOURCE_LENGTH, str.c_str());

    result.SourceIdentifier = GetLUID();

    return result;
}

static TicketData GetTicketData(const std::string& serviceTicketText)
{
    return TicketData(serviceTicketText.begin(), serviceTicketText.end());
}

TestLsaLogonUserDataManagerWithInit::TestLsaLogonUserDataManagerWithInit()
{
    KerberosServer::SystemApiWrapperPtr systemWrapper(new Mock::MockSystemApiWrapper(0));

    m_lsaLogonUserDataManager.reset(new LsaLogonUserDataManager(systemWrapper,
        GetParam().initial_origin,
        GetParam().initial_srcModuleIdentifies));
}

TEST_P(TestLsaLogonUserDataManagerWithInit, DataRequests)
{
    auto expectedResult = GetParam();
    auto initialParams = GetParam();

    ULONG logonTicketLength = 0;

    KERB_TICKET_LOGON* logonTicket = m_lsaLogonUserDataManager->GetKerbTicketLogon(initialParams.initial_serviceTicket, initialParams.initial_krbtgtTicket, logonTicketLength);

    EXPECT_EQ(logonTicket->MessageType, expectedResult.final_MessageType);
    EXPECT_EQ(logonTicket->Flags, expectedResult.final_Flags);
    EXPECT_EQ(memcmp(logonTicket->ServiceTicket, expectedResult.final_ServiceTicket, GetParam().initial_serviceTicket.size()), 0);
    EXPECT_EQ(logonTicket->ServiceTicketLength, expectedResult.final_ServiceTicketLength);
    EXPECT_EQ(memcmp(logonTicket->TicketGrantingTicket, expectedResult.final_TicketGrantingTicket, GetParam().initial_krbtgtTicket.size()), 0);
    EXPECT_EQ(logonTicket->TicketGrantingTicketLength, expectedResult.final_TicketGrantingTicketLength);

    LSA_STRING lsaOrigin;
    m_lsaLogonUserDataManager->InitLsaOrigin(lsaOrigin);
    EXPECT_EQ(lsaOrigin.Length, expectedResult.final_lsaOrigin.Length);
    EXPECT_EQ(lsaOrigin.MaximumLength, expectedResult.final_lsaOrigin.MaximumLength);
    EXPECT_EQ(strncmp(lsaOrigin.Buffer, expectedResult.final_lsaOrigin.Buffer, expectedResult.final_lsaOrigin.Length), 0);

    TOKEN_SOURCE srcContext;
    m_lsaLogonUserDataManager->InitTokenSource(srcContext);
    EXPECT_EQ(strcmp(srcContext.SourceName, expectedResult.final_srcContext.SourceName), 0);
    EXPECT_EQ(srcContext.SourceIdentifier.HighPart, expectedResult.final_srcContext.SourceIdentifier.HighPart);
    EXPECT_EQ(srcContext.SourceIdentifier.LowPart, expectedResult.final_srcContext.SourceIdentifier.LowPart);

    PLUID logonId = m_lsaLogonUserDataManager->GetDefaultLogonId();
    EXPECT_EQ(memcmp(logonId, &expectedResult.final_LogonId, sizeof(LUID)), 0);

    PQUOTA_LIMITS quotaLimits = m_lsaLogonUserDataManager->GetDefaultQuotaLimits();
    EXPECT_EQ(memcmp(quotaLimits, &expectedResult.final_quota_limits, sizeof(QUOTA_LIMITS)), 0);
}

static std::string serviceTicketText_1 = "It is some service ticket";
static std::string serviceTicketText_2 = "It is other service ticket";

static std::string krbtgtTicketText_1 = "It is some krbtgt ticket";
static std::string krbtgtTicketText_2 = "It is other krbtgt ticket";

static std::string origin_1 = "exmln";
static std::string origin_2 = "othln";

static std::string srcModuleIdentifies_1 = "exm";
static std::string srcModuleIdentifies_2 = "oth";

INSTANTIATE_TEST_CASE_P(Default, TestLsaLogonUserDataManagerWithInit,
    testing::Values(LsaLogonUser_state{ origin_1,
                                        srcModuleIdentifies_1,
                                        GetTicketData(serviceTicketText_1.c_str()),
                                        GetTicketData(krbtgtTicketText_1.c_str()),
                                        KERB_LOGON_SUBMIT_TYPE::KerbTicketLogon,
                                        0,
                                        static_cast<ULONG>(serviceTicketText_1.size()),
                                        static_cast<ULONG>(krbtgtTicketText_1.size()),
                                        (PUCHAR)serviceTicketText_1.c_str(),
                                        (PUCHAR)krbtgtTicketText_1.c_str(),
                                        CreateLsaStringFromString(origin_1),
                                        CreateTokenSourceFromString(srcModuleIdentifies_1),
                                        GetLUID(),
                                        GetQUOTA_LIMITS()
                                      },
                    LsaLogonUser_state{ origin_2,
                                        srcModuleIdentifies_2,
                                        GetTicketData(serviceTicketText_2.c_str()),
                                        GetTicketData(krbtgtTicketText_2.c_str()),
                                        KERB_LOGON_SUBMIT_TYPE::KerbTicketLogon,
                                        0,
                                        static_cast<ULONG>(serviceTicketText_2.size()),
                                        static_cast<ULONG>(krbtgtTicketText_2.size()),
                                        (PUCHAR)serviceTicketText_2.c_str(),
                                        (PUCHAR)krbtgtTicketText_2.c_str(),
                                        CreateLsaStringFromString(origin_2),
                                        CreateTokenSourceFromString(srcModuleIdentifies_2),
                                        GetLUID(),
                                        GetQUOTA_LIMITS()
                                       } 
));

TEST(LsaLogonUserDataManager, failed_OnRetrievingTokenSource)
{
    int failingByReason = Mock::SystemApiWrapperFailingReason::FailedAllocateLocallyUniqueId;
    SystemApiWrapperPtr systemApiWrapper(new Mock::MockSystemApiWrapper(failingByReason));

    LsaLogonUserDataManagerPtr lsaLogonUserDataManager(new LsaLogonUserDataManager(systemApiWrapper, origin_1, srcModuleIdentifies_1));

    TOKEN_SOURCE srcContext;
    ASSERT_THROW(lsaLogonUserDataManager->InitTokenSource(srcContext), KerbException::KerbServerException);
}

TEST(LsaLogonUserDataManager, success_OnCutting_TOKEN_SOURCE_SourceName)
{
    int failingByReason = 0;
    SystemApiWrapperPtr systemApiWrapper(new Mock::MockSystemApiWrapper(failingByReason));

    std::string bigSrcModuleIdentifies("some very long source module identifier");
    std::string expectedSourceName("some ve");
    LUID expectedSourceIdentifier = GetLUID();

    LsaLogonUserDataManagerPtr lsaLogonUserDataManager(new LsaLogonUserDataManager(systemApiWrapper, origin_1, bigSrcModuleIdentifies));

    TOKEN_SOURCE srcContext;
    ASSERT_NO_THROW(lsaLogonUserDataManager->InitTokenSource(srcContext));

    EXPECT_EQ(strcmp(srcContext.SourceName, expectedSourceName.c_str()), 0);
    EXPECT_EQ(srcContext.SourceIdentifier.HighPart, expectedSourceIdentifier.HighPart);
    EXPECT_EQ(srcContext.SourceIdentifier.LowPart, expectedSourceIdentifier.LowPart);
}