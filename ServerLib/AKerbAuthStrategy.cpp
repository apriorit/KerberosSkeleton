#include "AKerbAuthStrategy.h"
#include "AKerbAuthenticator.h"
#include "KerberosException.h"

using namespace KerberosServer;

AKerbAuthStrategy::AKerbAuthStrategy(TicketData& serviceTicket, TicketData& krbtgtTicket) :
    m_serviceTicket(std::move(serviceTicket)),
    m_krbtgtTicket(std::move(krbtgtTicket)),
    m_hToken(nullptr, &::CloseHandle)
{
}

ClientResponsePtr AKerbAuthStrategy::CreateResponse()
{
    ClientResponsePtr response;

    try
    {
        KerbAuthenticatorPtr kerbAuthenticator = GetKerbAuthenticator();
        m_hToken = TokenHandlePtr(kerbAuthenticator->Authenticate(GetServiceTicket(), GetKrbtgtTicket()), &::CloseHandle);
        response = PackSpnToResponse("success authentication");
    }
    catch (const KerbException::KerberosException& /*ex*/)
    {
        //TO DO: write to the log file here.
        response = PackSpnToResponse("failed authentication");
        m_hToken.reset(nullptr);
    }

    return response;
}

const TicketData& AKerbAuthStrategy::GetServiceTicket()const
{
    return m_serviceTicket;
}

const TicketData& AKerbAuthStrategy::GetKrbtgtTicket()const
{
    return m_krbtgtTicket;
}