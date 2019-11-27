#include "KerbAuthStrategy.h"
#include "KerbAuthenticator.h"
#include "Mock/MockSecur32Wrapper.h"
#include "ClientResponse.h"

using namespace KerberosServer;
using namespace Mock;

KerbAuthStrategy::KerbAuthStrategy(TicketData& serviceTicket, TicketData& krbtgtTicket, int failingReason) :
    AKerbAuthStrategy(serviceTicket, krbtgtTicket),
    m_authenticationResult("failed authentication"),
    m_failingReason(failingReason)
{
}

const std::string& KerbAuthStrategy::GetAuthenticationResult()const
{
    return m_authenticationResult;
}

ClientResponsePtr KerbAuthStrategy::PackSpnToResponse(const std::string& responseStr)
{
    m_authenticationResult = responseStr;
    return std::make_unique<ClientResponse>();
}

Secur32::Secur32WrapperPtr KerbAuthStrategy::GetSecur32Wrapper()const
{
    if (m_failingReason == Failed_LsaLogonUser)
    {
        return std::make_unique<Mock::MockSecur32Wrapper>(Secur32WrapperFailingReason::LsaLogonUser);
    }
    if (m_failingReason == Failed_CreatingSecur32Wrapper)
    {
        return std::make_unique<Mock::MockSecur32Wrapper>(Secur32WrapperFailingReason::CreateInstance);
    }

    return std::make_unique<Mock::MockSecur32Wrapper>(0);
}

KerbAuthenticatorPtr KerbAuthStrategy::GetKerbAuthenticator()const
{
    return KerbAuthenticatorPtr(new KerbAuthenticator(GetSecur32Wrapper()));
}