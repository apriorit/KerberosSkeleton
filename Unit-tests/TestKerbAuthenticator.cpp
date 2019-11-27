#include "gtest/gtest.h"
#include "KerbAuthenticator.h"
#include "Mock/MockSecur32Wrapper.h"
#include "KerberosException.h"
#include <memory>

using namespace KerberosServer;
using namespace Mock;

typedef std::unique_ptr<Secur32::ISecur32Wrapper> Secur32WrapperPtr;
typedef std::unique_ptr<AKerbAuthenticator> KerbAuthenticatorPtr;

static TicketData CreateKerbTicketFromString(const std::string& strTicket)
{
    return TicketData(strTicket.begin(), strTicket.end());
}

TEST(TestKerbAuthenticator, success)
{
    Secur32WrapperPtr secur32Wrapper(new MockSecur32Wrapper(0));
    KerbAuthenticatorPtr kerbAuthenticator;
    TicketData serviceTicket = CreateKerbTicketFromString("Some server ticket");
    TicketData krbtgtTicket = CreateKerbTicketFromString("Some krbtgt ticket");

    HANDLE hToken = INVALID_HANDLE_VALUE;

    ASSERT_NO_THROW(kerbAuthenticator.reset(new KerbAuthenticator(std::move(secur32Wrapper))));
    ASSERT_NO_THROW(hToken = kerbAuthenticator->Authenticate(serviceTicket, krbtgtTicket));
    ASSERT_TRUE(hToken != nullptr);
}

TEST(TestKerbAuthenticator, invalid_Secur32WrapperObj)
{
    Secur32WrapperPtr secur32Wrapper;
    KerbAuthenticatorPtr kerbAuthenticator;

    ASSERT_THROW(kerbAuthenticator.reset(new KerbAuthenticator(std::move(secur32Wrapper))), KerbException::KerberosException);
}

TEST(TestKerbAuthenticator, invalid_ThrowExeptionOnLsaConnectUntrusted)
{
    int functionsToFailing = 0;
    functionsToFailing |= Secur32WrapperFailingReason::LsaConnectUntrusted;

    Secur32WrapperPtr secur32Wrapper(new MockSecur32Wrapper(functionsToFailing));
    KerbAuthenticatorPtr kerbAuthenticator;

    ASSERT_THROW(kerbAuthenticator.reset(new KerbAuthenticator(std::move(secur32Wrapper))), KerbException::KerberosException);
}

TEST(TestKerbAuthenticator, invalid_ThrowExeptionOnLsaLookupAuthenticationPackage)
{
    int functionsToFailing = 0;
    functionsToFailing |= Secur32WrapperFailingReason::LsaLookupAuthenticationPackage;

    Secur32WrapperPtr secur32Wrapper(new MockSecur32Wrapper(functionsToFailing));
    KerbAuthenticatorPtr kerbAuthenticator;

    ASSERT_THROW(kerbAuthenticator.reset(new KerbAuthenticator(std::move(secur32Wrapper))), KerbException::KerberosException);
}

TEST(TestKerbAuthenticator, invalid_ThrowExeptionOnLsaLogonUser)
{
    int functionsToFailing = 0;
    functionsToFailing |= Secur32WrapperFailingReason::LsaLogonUser;

    Secur32WrapperPtr secur32Wrapper(new MockSecur32Wrapper(functionsToFailing));
    KerbAuthenticatorPtr kerbAuthenticator;
    TicketData serviceTicket = CreateKerbTicketFromString("Some service ticket");
    TicketData krbtgtTicket = CreateKerbTicketFromString("Some krbtgt ticket");

    ASSERT_NO_THROW(kerbAuthenticator.reset(new KerbAuthenticator(std::move(secur32Wrapper))));
    ASSERT_THROW(kerbAuthenticator->Authenticate(serviceTicket, krbtgtTicket), KerbException::KerberosException);
}

TEST(TestKerbAuthenticator, invalid_FaileOnLsaDeregisterLogonProcess)
{
    int functionsToFailing = 0;
    functionsToFailing |= Secur32WrapperFailingReason::LsaDeregisterLogonProcess;

    Secur32WrapperPtr secur32Wrapper(new MockSecur32Wrapper(functionsToFailing));
    KerbAuthenticatorPtr kerbAuthenticator;
    TicketData serviceTicket = CreateKerbTicketFromString("Some service ticket");
    TicketData krbtgtTicket = CreateKerbTicketFromString("Some krbtgt ticket");

    ASSERT_NO_THROW(kerbAuthenticator.reset(new KerbAuthenticator(std::move(secur32Wrapper))));
    ASSERT_NO_THROW(kerbAuthenticator->Authenticate(serviceTicket, krbtgtTicket));
}

TEST(TestKerbAuthenticator, invalid_FaileOnLsaFreeReturnBuffer)
{
    int functionsToFailing = 0;
    functionsToFailing |= Secur32WrapperFailingReason::LsaFreeReturnBuffer;

    Secur32WrapperPtr secur32Wrapper(new MockSecur32Wrapper(functionsToFailing));
    KerbAuthenticatorPtr kerbAuthenticator;
    TicketData serviceTicket = CreateKerbTicketFromString("Some service ticket");
    TicketData krbtgtTicket = CreateKerbTicketFromString("Some krbtgt ticket");

    ASSERT_NO_THROW(kerbAuthenticator.reset(new KerbAuthenticator(std::move(secur32Wrapper))));
    ASSERT_NO_THROW(kerbAuthenticator->Authenticate(serviceTicket, krbtgtTicket));
}
