#include "gtest/gtest.h"
#include "KerbAuthStrategy.h"
#include "Mock/MockSystemApiWrapper.h"
#include "KerbServerException.h"

using namespace KerberosServer;
using namespace Mock;

typedef std::unique_ptr<KerbAuthStrategy> KerbAuthStrategyPtr;

static TicketData CreateKerbTicketFromString(const std::string& strTicket)
{
    return TicketData(strTicket.begin(), strTicket.end());
}

TEST(TestKerbAuthStrategy, success)
{
    TicketData serviceTicket = CreateKerbTicketFromString("Some server ticket");
    TicketData krbtgtTicket = CreateKerbTicketFromString("Some krbtgt ticket");

    ClientResponsePtr response;
    KerbAuthStrategyPtr kerbAuthStrategy(new KerbAuthStrategy(serviceTicket, krbtgtTicket, 0));

    ASSERT_NO_THROW(response = kerbAuthStrategy->CreateResponse());
    const std::string& authenticationResult = kerbAuthStrategy->GetAuthenticationResult();
    ASSERT_EQ(authenticationResult, "success authentication");
}

TEST(TestKerbAuthStrategy, failed_OnLsaLogonUser)
{
    int functionToFailing = 0;
    functionToFailing = KerbAuthStrategyFailingReason::Failed_LsaLogonUser;

    TicketData serviceTicket = CreateKerbTicketFromString("Some server ticket");
    TicketData krbtgtTicket = CreateKerbTicketFromString("Some krbtgt ticket");

    ClientResponsePtr response;
    KerbAuthStrategyPtr kerbAuthStrategy(new KerbAuthStrategy(serviceTicket, krbtgtTicket, functionToFailing));

    ASSERT_NO_THROW(response = kerbAuthStrategy->CreateResponse());
    const std::string& authenticationResult = kerbAuthStrategy->GetAuthenticationResult();
    ASSERT_EQ(authenticationResult, "failed authentication");
}

TEST(TestKerbAuthStrategy, failed_OnCreatingSecur32Wrapper)
{
    int functionToFailing = 0;
    functionToFailing = KerbAuthStrategyFailingReason::Failed_CreatingSecur32Wrapper;

    TicketData serviceTicket = CreateKerbTicketFromString("Some server ticket");
    TicketData krbtgtTicket = CreateKerbTicketFromString("Some krbtgt ticket");

    ClientResponsePtr response;
    KerbAuthStrategyPtr kerbAuthStrategy(new KerbAuthStrategy(serviceTicket, krbtgtTicket, functionToFailing));

    ASSERT_NO_THROW(response = kerbAuthStrategy->CreateResponse());
    const std::string& authenticationResult = kerbAuthStrategy->GetAuthenticationResult();
    ASSERT_EQ(authenticationResult, "failed authentication");
}