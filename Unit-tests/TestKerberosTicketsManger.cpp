#include "gtest/gtest.h"
#include "KerberosTicketsManger.h"
#include "Mock/MockSecur32Wrapper.h"
#include "KerberosException.h"
#include <memory>

using namespace KerberosClient;
using namespace Mock;

typedef std::shared_ptr<Secur32::ISecur32Wrapper> Secur32WrapperPtr;
typedef std::unique_ptr<KerberosTicketsManger> KerberosTicketsMangerPtr;

TEST(TestKerberosTicketsManger, success)
{
    Secur32WrapperPtr secur32Wrapper(new MockSecur32Wrapper(0));
    KerberosTicketsMangerPtr ticketManager;
    TicketData retTicketData;

    ASSERT_NO_THROW(ticketManager.reset(new KerberosTicketsManger(secur32Wrapper)));
    ASSERT_NO_THROW(ticketManager->RequestTicketFromSystem(retTicketData, L"host/DnsHostname.DnsDomain@DnsDomain"));
}

TEST(TestKerberosTicketsManger, successOnRetrievingKrbtgtTicket)
{
    Secur32WrapperPtr secur32Wrapper(new MockSecur32Wrapper(0));
    KerberosTicketsMangerPtr ticketManager;
    TicketData retTicketData;

    ASSERT_NO_THROW(ticketManager.reset(new KerberosTicketsManger(secur32Wrapper)));
    ASSERT_NO_THROW(ticketManager->RequestTicketFromSystem(retTicketData, L"krbtgt"));
}

TEST(TestKerberosTicketsManger, invalid_Secur32WrapperObj)
{
    Secur32WrapperPtr secur32Wrapper;
    KerberosTicketsMangerPtr ticketManager;

    ASSERT_THROW(ticketManager.reset(new KerberosTicketsManger(secur32Wrapper)), KerbException::KerberosException);
}

TEST(TestKerberosTicketsManger, invalid_ThrowExeptionOnLsaConnectUntrusted)
{
    int functionsToFailing = 0;
    functionsToFailing |= Secur32WrapperFailingReason::LsaConnectUntrusted;

    Secur32WrapperPtr secur32Wrapper(new MockSecur32Wrapper(functionsToFailing));
    KerberosTicketsMangerPtr ticketManager;

    ASSERT_THROW(ticketManager.reset(new KerberosTicketsManger(secur32Wrapper)), KerbException::KerberosException);
}

TEST(TestKerberosTicketsManger, invalid_ThrowExeptionOnLsaLookupAuthenticationPackage)
{
    int functionsToFailing = 0;
    functionsToFailing |= Secur32WrapperFailingReason::LsaLookupAuthenticationPackage;

    Secur32WrapperPtr secur32Wrapper(new MockSecur32Wrapper(functionsToFailing));
    KerberosTicketsMangerPtr ticketManager;

    ASSERT_THROW(ticketManager.reset(new KerberosTicketsManger(secur32Wrapper)), KerbException::KerberosException);
}

TEST(TestKerberosTicketsManger, invalid_ThrowExeptionOnLsaCallAuthenticationPackage)
{
    int functionsToFailing = 0;
    functionsToFailing |= Secur32WrapperFailingReason::LsaCallAuthenticationPackage;

    Secur32WrapperPtr secur32Wrapper(new MockSecur32Wrapper(functionsToFailing));
    KerberosTicketsMangerPtr ticketManager;
    TicketData retTicketData;


    ASSERT_NO_THROW(ticketManager.reset(new KerberosTicketsManger(secur32Wrapper)));
    ASSERT_THROW(ticketManager->RequestTicketFromSystem(retTicketData, L"host/DnsHostname.DnsDomain@DnsDomain"), KerbException::KerberosException);
}

TEST(TestKerberosTicketsManger, invalid_ThrowExeptionOnLsaCallAuthenticationPackageProtocol)
{
    int functionsToFailing = 0;
    functionsToFailing |= Secur32WrapperFailingReason::LsaCallAuthenticationPackageProtocol;

    Secur32WrapperPtr secur32Wrapper(new MockSecur32Wrapper(functionsToFailing));
    KerberosTicketsMangerPtr ticketManager;
    TicketData retTicketData;


    ASSERT_NO_THROW(ticketManager.reset(new KerberosTicketsManger(secur32Wrapper)));
    ASSERT_THROW(ticketManager->RequestTicketFromSystem(retTicketData, L"host/DnsHostname.DnsDomain@DnsDomain"), KerbException::KerberosException);
}
//
TEST(TestKerberosTicketsManger, invalid_ThrowExeptionOnLsaCallAuthenticationPackage_ResonseIsNULL)
{
    int functionsToFailing = 0;
    functionsToFailing |= Secur32WrapperFailingReason::LsaCallAuthenticationPackageResonseIsNULL;

    Secur32WrapperPtr secur32Wrapper(new MockSecur32Wrapper(functionsToFailing));
    KerberosTicketsMangerPtr ticketManager;
    TicketData retTicketData;


    ASSERT_NO_THROW(ticketManager.reset(new KerberosTicketsManger(secur32Wrapper)));
    ASSERT_THROW(ticketManager->RequestTicketFromSystem(retTicketData, L"host/DnsHostname.DnsDomain@DnsDomain"), KerbException::KerberosException);
}
TEST(TestKerberosTicketsManger, invalid_FaileOnLsaDeregisterLogonProcess)
{
    int functionsToFailing = 0;
    functionsToFailing |= Secur32WrapperFailingReason::LsaDeregisterLogonProcess;

    Secur32WrapperPtr secur32Wrapper(new MockSecur32Wrapper(functionsToFailing));
    KerberosTicketsMangerPtr ticketManager;
    TicketData retTicketData;

    ASSERT_NO_THROW(ticketManager.reset(new KerberosTicketsManger(secur32Wrapper)));
    ASSERT_NO_THROW(ticketManager->RequestTicketFromSystem(retTicketData, L"krbtgt"));
}

TEST(TestKerberosTicketsManger, invalid_FaileOnLsaFreeReturnBuffer)
{
    int functionsToFailing = 0;
    functionsToFailing |= Secur32WrapperFailingReason::LsaFreeReturnBuffer;

    Secur32WrapperPtr secur32Wrapper(new MockSecur32Wrapper(functionsToFailing));
    KerberosTicketsMangerPtr ticketManager;
    TicketData retTicketData;

    ASSERT_NO_THROW(ticketManager.reset(new KerberosTicketsManger(secur32Wrapper)));
    ASSERT_NO_THROW(ticketManager->RequestTicketFromSystem(retTicketData, L"krbtgt"));
}
