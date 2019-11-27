#include "AKerbAuthRequestDecorator.h"
#include "KerberosTicketsManger.h"
#include "SpnValidator.h"
#include "KerberosException.h"

using namespace KerberosClient;
using namespace KerbException;

AKerbAuthRequestDecorator::AKerbAuthRequestDecorator(ServerRequestPtr request, Secur32::Secur32WrapperPtr secur32Wrapper, SpnValidatorPtr spnValidator) :
    ARequestDecorator(std::move(request)),
    m_spnValidator(std::move(spnValidator)),
    m_secur32Wrapper(std::move(secur32Wrapper))
{
}

ServerResponsePtr AKerbAuthRequestDecorator::SendRequest()
{
    IServerRequest* spnRequest = GetRequest();
    ServerResponsePtr spnResponse;
    TicketData serviceTicket;
    TicketData krbtgtTicket;
    const ULONG ticketFlags = 0x60A00000; //forwardable forwarded renewable pre_authent

    static const std::wstring krbtgtName = L"krbtgt";
    static const std::wstring SPN_KEY = L"SPN";

    spnResponse = spnRequest->SendRequest();
    const std::wstring servicePrincipalName = spnResponse->GetStringDataFromResponse(SPN_KEY);

    bool isSpnDataValid = m_spnValidator->Validate(servicePrincipalName);

    if (!isSpnDataValid)
    {
        throw KerberosException(
            KerberosException::ErrorType::INVALID_SPN_DATA,
            "AKerbAuthRequestDecorator::SendRequest: spn data is invalid"
        );
    }

    typedef std::unique_ptr<KerberosTicketsManger> KerberosTicketsMangerPtr;

    const KerberosTicketsMangerPtr kerbTicketsManger(new KerberosTicketsManger(m_secur32Wrapper));
    kerbTicketsManger->SetTicketFlags(ticketFlags);
    kerbTicketsManger->RequestTicketFromSystem(serviceTicket, servicePrincipalName);
    kerbTicketsManger->SetCacheOptions(KERB_RETRIEVE_TICKET_AS_KERB_CRED);
    kerbTicketsManger->RequestTicketFromSystem(krbtgtTicket, krbtgtName);

    const ServerRequestPtr authRequest = PackTicketsToRequest(serviceTicket, krbtgtTicket);

    ServerResponsePtr authResponse = authRequest->SendRequest();

    return authResponse;
}