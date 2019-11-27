#ifndef SECUR32_WRAPPER
#define SECUR32_WRAPPER

#include "ISecur32Wrapper.h"
#include <string>

namespace Secur32
{
    class Secur32Wrapper : public ISecur32Wrapper
    {
    public:
        Secur32Wrapper();
        virtual ~Secur32Wrapper();

    public: //ISecur32Wrapper interface implementation
        virtual NTSTATUS NTAPI LsaCallAuthenticationPackage(
            HANDLE lsaHandle,
            ULONG authenticationPackage,
            PVOID submitBuffer,
            ULONG submitBufferLength,
            PVOID* retBuffer,
            PULONG retBufferLength,
            PNTSTATUS status
        );
        virtual NTSTATUS NTAPI LsaConnectUntrusted(PHANDLE lsaHandle);
        virtual NTSTATUS NTAPI LsaDeregisterLogonProcess(HANDLE lsaHandle);
        virtual NTSTATUS NTAPI LsaFreeReturnBuffer(PVOID buffer);
        virtual NTSTATUS NTAPI LsaLogonUser(
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
        );
        virtual NTSTATUS NTAPI LsaLookupAuthenticationPackage(HANDLE lsaHandle, PLSA_STRING packageName, PULONG authenticationPackage);

    private:
        void InitializeLsaFunctions();

    private:
        typedef NTSTATUS(NTAPI *pFuncLsaCallAuthenticationPackage)(
            HANDLE lsaHandle,
            ULONG authenticationPackage,
            PVOID submitBuffer,
            ULONG submitBufferLength,
            PVOID* retBuffer,
            PULONG retBufferLength,
            PNTSTATUS status
            );

        typedef NTSTATUS(NTAPI *pFuncLsaConnectUntrusted)(
            PHANDLE lsaHandle
            );

        typedef NTSTATUS(NTAPI *pFuncLsaDeregisterLogonProcess)(
            HANDLE lsaHandle
            );

        typedef NTSTATUS(NTAPI *pFuncLsaFreeReturnBuffer)(
            PVOID buffer
            );

        typedef NTSTATUS(NTAPI *pFuncLsaLogonUser)(
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
            );

        typedef NTSTATUS(NTAPI *pFuncLsaLookupAuthenticationPackage)(
            HANDLE lsaHandle,
            PLSA_STRING packageName,
            PULONG authenticationPackage
            );

    private:
        HMODULE m_secur32HModule;
        pFuncLsaCallAuthenticationPackage m_lsaCallAuthenticationPackageFunction;
        pFuncLsaConnectUntrusted m_lsaConnectUntrustedFunction;
        pFuncLsaDeregisterLogonProcess m_lsaDeregisterLogonProcessFunction;
        pFuncLsaFreeReturnBuffer m_lsaFreeReturnBufferFunction;
        pFuncLsaLogonUser m_lsaLogonUserFunction;
        pFuncLsaLookupAuthenticationPackage m_lsaLookupAuthenticationPackageFunction;

    private:
        static std::string SECUR32_DLL_NAME;
    };
}

#endif // !SECUR32_WRAPPER