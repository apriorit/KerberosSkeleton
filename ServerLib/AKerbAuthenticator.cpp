#include "AKerbAuthenticator.h"
#include "KerberosException.h"
#include "LsaLogonUserDataManager.h"

using namespace KerberosServer;
using namespace KerbException;

AKerbAuthenticator::AKerbAuthenticator(Secur32::Secur32WrapperPtr secur32Wrapper) :
    m_secur32Wrapper(std::move(secur32Wrapper))
{
    if (!m_secur32Wrapper)
    {
        throw KerberosException(
            KerberosException::ErrorType::INVALID_SECUR32WRAPPER_INSATNCE,
            "AKerbAuthenticator::AKerbAuthenticator: Attempts create the instance with invalid ISecur32Wrapper object."
        );
    }
    InitializeUntrustedConnect();
}

void AKerbAuthenticator::InitializeUntrustedConnect()
{
    HANDLE hLsa = NULL;
    NTSTATUS ticketsStatus = m_secur32Wrapper->LsaConnectUntrusted(&hLsa);

    if (STATUS_SUCCESS != ticketsStatus)
    {
        throw KerberosException(
            KerberosException::ErrorType::FAILED_LSACONNECTUNTRUSTED,
            "KerbAuthenticator::InitializeUntrustedConnect: LsaConnectUntrusted failed"
        );
    }

    m_hLsa = LsaHandlePtr(hLsa, GetLsaHandleDeleter());

    LSA_STRING lsaStrAuthPkg;
    lsaStrAuthPkg.Length = static_cast<USHORT>(strlen(MICROSOFT_KERBEROS_NAME_A));
    lsaStrAuthPkg.MaximumLength = static_cast<USHORT>(strlen(MICROSOFT_KERBEROS_NAME_A));
    lsaStrAuthPkg.Buffer = MICROSOFT_KERBEROS_NAME_A;

    ticketsStatus = m_secur32Wrapper->LsaLookupAuthenticationPackage(m_hLsa.get(), &lsaStrAuthPkg, &m_authPkgId);

    if (STATUS_SUCCESS != ticketsStatus)
    {
        throw KerberosException(
            KerberosException::ErrorType::FAILED_LSALOOKUPAUTHENTICATIONPACKAGE,
            "KerbAuthenticator::InitializeUntrustedConnect: LsaLookupAuthenticationPackage failed"
        );
    }
}

AKerbAuthenticator::LsaHandleDeleter AKerbAuthenticator::GetLsaHandleDeleter()const
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

AKerbAuthenticator::LsaBufferDeleter AKerbAuthenticator::GetLsaBufferDeleter()const
{
    LsaBufferDeleter lsaBufferDeleter = [&secur32Wrapper = m_secur32Wrapper](PKERB_TICKET_PROFILE pBuf) {
        NTSTATUS status = secur32Wrapper->LsaFreeReturnBuffer(pBuf);
        if (status != STATUS_SUCCESS)
        {
            //TO DO: Write a message to the log file.
        }
    };
    return lsaBufferDeleter;
}

HANDLE AKerbAuthenticator::Authenticate(const TicketData& serviceTicket, const TicketData& krbtgtTicket)
{
    const LsaLogonUserDataManagerPtr lsaLogonUserDataManager(new LsaLogonUserDataManager(GetSystemApiWrapper(), GetOriginName(), GetSourceModuleIdentifier()));

    ULONG kerbTicketLogonLen = 0;
    KERB_TICKET_LOGON * pKerbTicketLogon = lsaLogonUserDataManager->GetKerbTicketLogon(serviceTicket, krbtgtTicket, kerbTicketLogonLen);

    LSA_STRING lsastrOriginName = {};
    lsaLogonUserDataManager->InitLsaOrigin(lsastrOriginName);

    TOKEN_SOURCE sourceContext = {};
    lsaLogonUserDataManager->InitTokenSource(sourceContext);

    PLUID logonId = lsaLogonUserDataManager->GetDefaultLogonId();
    PQUOTA_LIMITS quotaLimits = lsaLogonUserDataManager->GetDefaultQuotaLimits();

    PKERB_TICKET_PROFILE profileBuffer = NULL;
    HANDLE hToken = nullptr;
    ULONG profileBufferLen = -1;
    NTSTATUS subStatus = -1;

    NTSTATUS status = m_secur32Wrapper->LsaLogonUser(m_hLsa.get(),
                                                     &lsastrOriginName,
                                                     GetLogonType(),
                                                     m_authPkgId,
                                                     pKerbTicketLogon,
                                                     kerbTicketLogonLen,
                                                     NULL, // additional local groups
                                                     &sourceContext,
                                                     reinterpret_cast<PVOID*>(&profileBuffer),
                                                     &profileBufferLen,
                                                     logonId,
                                                     &hToken,
                                                     quotaLimits,
                                                     &subStatus);

    typedef std::unique_ptr<KERB_TICKET_PROFILE, LsaBufferDeleter> LsaProfileBufferPtr;
    const LsaProfileBufferPtr profileBufferPtr(profileBuffer, GetLsaBufferDeleter());

    if (status != STATUS_SUCCESS)
    {
        throw KerberosException(
            KerberosException::ErrorType::FAILED_LSALOGONUSER,
            "AKerbAuthenticator::Authenticate: LsaLogonUser failed");
    }

    return hToken;
}