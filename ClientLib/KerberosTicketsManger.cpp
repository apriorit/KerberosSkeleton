#include "KerberosTicketsManger.h"
#include "KerbRetrieveTktRequest.h"
#include "KerberosException.h"
#include "Secur32Wrapper.h"


using namespace KerberosClient;
using namespace KerbException;

const unsigned long KerberosTicketsManger::DEFAULT_AUTH_PKG_ID = static_cast<unsigned long>(-1);

KerberosTicketsManger::KerberosTicketsManger(const std::shared_ptr<Secur32::ISecur32Wrapper>& secur32Wrapper) :
    m_secur32Wrapper(secur32Wrapper),
    m_authPkgId(DEFAULT_AUTH_PKG_ID)
{
    if (!m_secur32Wrapper)
    {
        throw KerberosException(
            KerberosException::ErrorType::INVALID_SECUR32WRAPPER_INSATNCE,
            "KerberosTicketsManger::KerberosTicketsManger: Attempts create the instance with invalid ISecur32Wrapper object."
        );
    }
    InitializeUntrustedConnect();
}

void KerberosTicketsManger::InitializeUntrustedConnect()
{
    HANDLE hLsa = NULL;
    NTSTATUS ticketsStatus = m_secur32Wrapper->LsaConnectUntrusted(&hLsa);

    if (STATUS_SUCCESS != ticketsStatus)
    {
        throw KerberosException(
            KerberosException::ErrorType::FAILED_LSACONNECTUNTRUSTED,
            "KerberosTicketsManger::InitializeConnection: LsaConnectUntrusted failed"
        );
    }

    m_hLsa = LsaHandlePtr(hLsa, GetLsaHandleDeleter());

    LSA_STRING lsaStrAuthPkg = {};
    lsaStrAuthPkg.Length = static_cast<USHORT>(strlen(MICROSOFT_KERBEROS_NAME_A));
    lsaStrAuthPkg.MaximumLength = static_cast<USHORT>(strlen(MICROSOFT_KERBEROS_NAME_A));
    lsaStrAuthPkg.Buffer = MICROSOFT_KERBEROS_NAME_A;

    ticketsStatus = m_secur32Wrapper->LsaLookupAuthenticationPackage(m_hLsa.get(), &lsaStrAuthPkg, &m_authPkgId);

    if (STATUS_SUCCESS != ticketsStatus)
    {
        throw KerberosException(
            KerberosException::ErrorType::FAILED_LSALOOKUPAUTHENTICATIONPACKAGE,
            "KerberosTicketsManger::InitializeConnection: LsaLookupAuthenticationPackage failed"
        );
    }
}

void KerberosTicketsManger::RequestTicketFromSystem(TicketData& vecTicket, const std::wstring& tgtName)const
{
    KerbRetrieveTktRequest kerbRetrieveTktRequest(tgtName);
    ULONG responseLen = static_cast<ULONG>(-1);
    NTSTATUS protocolStatus = STATUS_ACCESS_DENIED;
    KERB_RETRIEVE_TKT_RESPONSE* pResp = NULL;

    kerbRetrieveTktRequest.SetTicketFlags(m_ticketFlags);
    kerbRetrieveTktRequest.SetCacheOptions(m_cacheOptions);

    NTSTATUS ticketsStatus = m_secur32Wrapper->LsaCallAuthenticationPackage(
        m_hLsa.get(),
        m_authPkgId,
        reinterpret_cast<PVOID>(kerbRetrieveTktRequest.GetRetrieveTktRequest()),
        kerbRetrieveTktRequest.Length(),
        reinterpret_cast<PVOID*>(&pResp),
        &responseLen,
        &protocolStatus
    );

    typedef std::unique_ptr<KERB_RETRIEVE_TKT_RESPONSE, LsaBufferDeleter> LsaBufferDeleterPtr;
    const LsaBufferDeleterPtr ticketPtr(pResp, GetLsaBufferDeleter());

    if (STATUS_SUCCESS != ticketsStatus)
    {
        throw KerberosException(KerberosException::ErrorType::FAILED_LSACALLAUTHENTICATIONPACKAGE, "KerberosTicketsManger::RequestTicketFromSystem: LsaCallAuthenticationPackage failed");
    }

    if (STATUS_SUCCESS != protocolStatus)
    {
        throw KerberosException(KerberosException::ErrorType::INVALID_PROTOCOLSTATUS, "KerberosTicketsManger::RequestTicketFromSystem: ProtocolStatus failed");
    }

    if (pResp == NULL)
    {
        throw KerberosException(KerberosException::ErrorType::INVALID_RETURNBUFFER, "KerberosTicketsManger::RequestTicketFromSystem: Buffer equal NULL in response");
    }

    UCHAR* pEncodedTicketBeg = ticketPtr->Ticket.EncodedTicket;
    UCHAR* pEncodedTicketEnd = ticketPtr->Ticket.EncodedTicket + ticketPtr->Ticket.EncodedTicketSize;
    vecTicket.clear();
    vecTicket.assign(pEncodedTicketBeg, pEncodedTicketEnd);
}

KerberosTicketsManger::LsaHandleDeleter KerberosTicketsManger::GetLsaHandleDeleter()const
{
    LsaHandleDeleter lsaDeleter = [&secur32Wrapper = m_secur32Wrapper](HANDLE hLsa) {
        NTSTATUS status = secur32Wrapper->LsaDeregisterLogonProcess(hLsa);
        if (status != STATUS_SUCCESS)
        {
            //TO DO: Write a message to the log file.
        }
    };

    return lsaDeleter;
}

KerberosTicketsManger::LsaBufferDeleter KerberosTicketsManger::GetLsaBufferDeleter()const
{
    LsaBufferDeleter lsaBufferDeleter = [&secur32Wrapper = m_secur32Wrapper](KERB_RETRIEVE_TKT_RESPONSE* pBuf) {
        NTSTATUS status = secur32Wrapper->LsaFreeReturnBuffer(pBuf);
        if (status != STATUS_SUCCESS)
        {
            //TO DO: Write a message to the log file.
        }
    };
    return lsaBufferDeleter;
}

void KerberosTicketsManger::SetTicketFlags(ULONG ticketFlags)
{
    m_ticketFlags = ticketFlags;
}

void KerberosTicketsManger::SetCacheOptions(ULONG cacheOptions)
{
    m_cacheOptions = cacheOptions;
}
