#include "MockSecur32Wrapper.h"
#include "KerberosException.h"

using namespace Mock;

MockSecur32Wrapper::MockSecur32Wrapper(int functionsToFailing) : m_functionsToFailing(functionsToFailing), m_isLsaLogonUser(false)
{
    if (IsFunctionFailed(Secur32WrapperFailingReason::CreateInstance))
    {
        throw KerbException::KerberosException(
                                               KerbException::KerberosException::ErrorType::FAILED_LOADINDSECUR32,
                                               "MockSecur32Wrapper::MockSecur32Wrapper: Failed loading the Secur32.dll");
    }
}

bool MockSecur32Wrapper::IsFunctionFailed(Secur32WrapperFailingReason mask)
{
    int result = static_cast<int>(mask) & m_functionsToFailing;
    return result != 0;
}

NTSTATUS NTAPI MockSecur32Wrapper::LsaCallAuthenticationPackage(
    HANDLE /*lsaHandle*/,
    ULONG /*authenticationPackage*/,
    PVOID /*submitBuffer*/,
    ULONG /*submitBufferLength*/,
    PVOID* retBuffer,
    PULONG /*retBufferLength*/,
    PNTSTATUS status
)
{
    if (IsFunctionFailed(Secur32WrapperFailingReason::LsaCallAuthenticationPackage))
    {
        return STATUS_ACCESS_DENIED;
    }

    *status = IsFunctionFailed(Secur32WrapperFailingReason::LsaCallAuthenticationPackageProtocol) ? STATUS_ACCESS_DENIED : STATUS_SUCCESS;

    if (IsFunctionFailed(Secur32WrapperFailingReason::LsaCallAuthenticationPackageResonseIsNULL))
    {
        *retBuffer = NULL;
        return STATUS_SUCCESS;
    }

    if (*status == STATUS_SUCCESS)
    {
        KERB_RETRIEVE_TKT_RESPONSE* result = new KERB_RETRIEVE_TKT_RESPONSE();
        *retBuffer = reinterpret_cast<PVOID>(result);
        m_isLsaLogonUser = false;
    }

    return STATUS_SUCCESS;
}

NTSTATUS NTAPI MockSecur32Wrapper::LsaConnectUntrusted(PHANDLE lsaHandle)
{
    if (IsFunctionFailed(Secur32WrapperFailingReason::LsaConnectUntrusted))
    {
        return STATUS_ACCESS_DENIED;
    }

    *lsaHandle = INVALID_HANDLE_VALUE;
    return STATUS_SUCCESS;
}

NTSTATUS NTAPI MockSecur32Wrapper::LsaDeregisterLogonProcess(HANDLE lsaHandle)
{
    if (IsFunctionFailed(Secur32WrapperFailingReason::LsaDeregisterLogonProcess))
    {
        return STATUS_ACCESS_DENIED;
    }

    return STATUS_SUCCESS;
}

NTSTATUS NTAPI MockSecur32Wrapper::LsaFreeReturnBuffer(PVOID buffer)
{
    if (buffer != NULL)
    {
        if (m_isLsaLogonUser)
        {
            PKERB_TICKET_PROFILE* result = reinterpret_cast<PKERB_TICKET_PROFILE*>(buffer);
            delete result;
        }
        else
        {
            KERB_RETRIEVE_TKT_RESPONSE* result = reinterpret_cast<KERB_RETRIEVE_TKT_RESPONSE*>(buffer);
            delete result;
        }

        buffer = NULL;
    }

    if (IsFunctionFailed(Secur32WrapperFailingReason::LsaFreeReturnBuffer))
    {
        return STATUS_ACCESS_DENIED;
    }

    return STATUS_SUCCESS;
}

NTSTATUS NTAPI MockSecur32Wrapper::LsaLogonUser(
    HANDLE /*lsaHandle*/,
    PLSA_STRING /*originName*/,
    SECURITY_LOGON_TYPE /*logonType*/,
    ULONG /*authenticationPackage*/,
    PVOID /*authenticationInfo*/,
    ULONG /*authenticationInfoLength*/,
    PTOKEN_GROUPS /*localGroups*/,
    PTOKEN_SOURCE /*sourceContext*/,
    PVOID* profileBuffer,
    PULONG /*profileBufferLength*/,
    PLUID /*logonId*/,
    PHANDLE token,
    PQUOTA_LIMITS /*quotas*/,
    PNTSTATUS /*subStatus*/
)
{
    if (IsFunctionFailed(Secur32WrapperFailingReason::LsaLogonUser))
    {
        return STATUS_ACCESS_DENIED;
    }

    *token = INVALID_HANDLE_VALUE;

    PKERB_TICKET_PROFILE* result = new PKERB_TICKET_PROFILE();
    *profileBuffer = reinterpret_cast<PVOID>(result);
    m_isLsaLogonUser = true;

    return STATUS_SUCCESS;
}

NTSTATUS NTAPI MockSecur32Wrapper::LsaLookupAuthenticationPackage(HANDLE /*lsaHandle*/, PLSA_STRING /*packageName*/, PULONG /*authenticationPackage*/)
{
    if (IsFunctionFailed(Secur32WrapperFailingReason::LsaLookupAuthenticationPackage))
    {
        return STATUS_ACCESS_DENIED;
    }

    return STATUS_SUCCESS;
}