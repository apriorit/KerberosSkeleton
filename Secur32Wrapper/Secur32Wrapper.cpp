#include "Secur32Wrapper.h"
#include "KerberosException.h"

namespace Secur32
{
    std::string Secur32Wrapper::SECUR32_DLL_NAME = "SECUR32.DLL";

    Secur32Wrapper::Secur32Wrapper()
    {
        m_secur32HModule = LoadLibraryA(SECUR32_DLL_NAME.c_str());
        if (m_secur32HModule == NULL)
        {
            throw KerbException::KerberosException(
                KerbException::KerberosException::ErrorType::FAILED_LOADINDSECUR32,
                "Secur32Wrapper::Secur32Wrapper: Failed loading the Secur32.dll"
            );
        }

        InitializeLsaFunctions();

        if ((m_lsaCallAuthenticationPackageFunction == NULL) ||
            (m_lsaConnectUntrustedFunction == NULL) ||
            (m_lsaDeregisterLogonProcessFunction == NULL) ||
            (m_lsaFreeReturnBufferFunction == NULL) ||
            (m_lsaLogonUserFunction == NULL) ||
            (m_lsaLookupAuthenticationPackageFunction == NULL))
        {
            FreeLibrary(m_secur32HModule);
            m_secur32HModule = NULL;

            throw KerbException::KerberosException(
                KerbException::KerberosException::ErrorType::FAILED_LOADINDSECUR32FUNCTIONS,
                "Secur32Wrapper::Secur32Wrapper: Failed loading a function from the Secur32.dll"
            );
        }
    }

    Secur32Wrapper::~Secur32Wrapper()
    {
        if (m_secur32HModule)
        {
            FreeLibrary(m_secur32HModule);
        }
    }

    NTSTATUS NTAPI Secur32Wrapper::LsaCallAuthenticationPackage(
        HANDLE lsaHandle,
        ULONG authenticationPackage,
        PVOID submitBuffer,
        ULONG submitBufferLength,
        PVOID* retBuffer,
        PULONG retBufferLength,
        PNTSTATUS status
    )
    {
        return m_lsaCallAuthenticationPackageFunction(lsaHandle, authenticationPackage, submitBuffer, submitBufferLength, retBuffer, retBufferLength, status);
    }

    NTSTATUS NTAPI Secur32Wrapper::LsaConnectUntrusted(PHANDLE lsaHandle)
    {
        return m_lsaConnectUntrustedFunction(lsaHandle);
    }

    NTSTATUS NTAPI Secur32Wrapper::LsaDeregisterLogonProcess(HANDLE lsaHandle)
    {
        return m_lsaDeregisterLogonProcessFunction(lsaHandle);
    }

    NTSTATUS NTAPI Secur32Wrapper::LsaFreeReturnBuffer(PVOID buffer)
    {
        if (buffer == NULL)
        {
            return STATUS_SUCCESS;
        }

        return m_lsaFreeReturnBufferFunction(buffer);
    }

    NTSTATUS NTAPI Secur32Wrapper::LsaLogonUser(
        HANDLE lsaHandle,
        PLSA_STRING originName,
        SECURITY_LOGON_TYPE logonType,
        ULONG authenticationPackage,
        PVOID authenticationInfo,
        ULONG authenticationInfoLength,
        PTOKEN_GROUPS localGroups,
        PTOKEN_SOURCE sourceContext,
        PVOID* profileBuffer,
        PULONG profileBufferLength,
        PLUID logonId,
        PHANDLE token,
        PQUOTA_LIMITS quotas,
        PNTSTATUS subStatus
    )
    {
        return m_lsaLogonUserFunction(lsaHandle, originName, logonType, authenticationPackage,
                                      authenticationInfo, authenticationInfoLength, localGroups,
                                      sourceContext, profileBuffer, profileBufferLength, logonId,
                                      token, quotas, subStatus);
    }

    NTSTATUS NTAPI Secur32Wrapper::LsaLookupAuthenticationPackage(HANDLE lsaHandle, PLSA_STRING packageName, PULONG authenticationPackage)
    {
        return m_lsaLookupAuthenticationPackageFunction(lsaHandle, packageName, authenticationPackage);
    }

    void Secur32Wrapper::InitializeLsaFunctions()
    {
        static const std::string LSA_CALL_AUTHENTICATION_PACKAGE = "LsaCallAuthenticationPackage";
        static const std::string LSA_CONNECT_UNTRUSTED = "LsaConnectUntrusted";
        static const std::string LSA_DEREGISTER_LOGON_PROCESS = "LsaDeregisterLogonProcess";
        static const std::string LSA_FREE_RETURN_BUFFER = "LsaFreeReturnBuffer";
        static const std::string LSA_LOGON_USER = "LsaLogonUser";
        static const std::string LSA_LOOKUP_AUTHENTICATION_PACKAGE = "LsaLookupAuthenticationPackage";


        m_lsaCallAuthenticationPackageFunction = (pFuncLsaCallAuthenticationPackage)
            GetProcAddress(m_secur32HModule, LSA_CALL_AUTHENTICATION_PACKAGE.c_str());
        m_lsaConnectUntrustedFunction = (pFuncLsaConnectUntrusted)
            GetProcAddress(m_secur32HModule, LSA_CONNECT_UNTRUSTED.c_str());
        m_lsaDeregisterLogonProcessFunction = (pFuncLsaDeregisterLogonProcess)
            GetProcAddress(m_secur32HModule, LSA_DEREGISTER_LOGON_PROCESS.c_str());
        m_lsaFreeReturnBufferFunction = (pFuncLsaFreeReturnBuffer)
            GetProcAddress(m_secur32HModule, LSA_FREE_RETURN_BUFFER.c_str());
        m_lsaLogonUserFunction = (pFuncLsaLogonUser)
            GetProcAddress(m_secur32HModule, LSA_LOGON_USER.c_str());
        m_lsaLookupAuthenticationPackageFunction = (pFuncLsaLookupAuthenticationPackage)
            GetProcAddress(m_secur32HModule, LSA_LOOKUP_AUTHENTICATION_PACKAGE.c_str());
    }
}