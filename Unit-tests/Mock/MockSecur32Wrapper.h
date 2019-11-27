#ifndef MOCK_SECUR32_WRAPPER
#define MOCK_SECUR32_WRAPPER

#include "ISecur32Wrapper.h"

namespace Mock
{
    enum Secur32WrapperFailingReason
    {
        LsaCallAuthenticationPackage = 1,
        LsaConnectUntrusted = 2,
        LsaDeregisterLogonProcess = 4,
        LsaFreeReturnBuffer = 8,
        LsaLogonUser = 16,
        LsaLookupAuthenticationPackage = 32,
        LsaCallAuthenticationPackageProtocol = 64,
        LsaCallAuthenticationPackageResonseIsNULL = 128,
        CreateInstance = 256
    };

    class MockSecur32Wrapper : public Secur32::ISecur32Wrapper
    {
    public:
        explicit MockSecur32Wrapper(int functionsToFailing);
    public:
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
        virtual NTSTATUS NTAPI LsaLookupAuthenticationPackage(HANDLE lsaHandle, PLSA_STRING packageName, PULONG authenticationPackageId);

    private:
        bool IsFunctionFailed(Secur32WrapperFailingReason mask);

    private:
        int m_functionsToFailing;
        bool m_isLsaLogonUser;
    };
}

#endif // MOCK_SECUR32_WRAPPER