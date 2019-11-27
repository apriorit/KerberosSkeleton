#ifndef I_SECUR32_WRAPPER
#define I_SECUR32_WRAPPER

#include <windows.h>
#include <ntsecapi.h>
#include <memory>
#include <vector>

typedef std::vector<unsigned char> TicketData;

namespace Secur32
{
#define STATUS_SUCCESS            ((NTSTATUS)0x00000000L)
#define STATUS_ACCESS_DENIED      ((NTSTATUS)0xC0000022L)

    class ISecur32Wrapper
    {
    public:
        virtual ~ISecur32Wrapper() {};

    public:
        virtual NTSTATUS NTAPI LsaCallAuthenticationPackage(
                                                            HANDLE lsaHandle,
                                                            ULONG authenticationPackage,
                                                            PVOID submitBuffer,
                                                            ULONG submitBufferLength,
                                                            PVOID* retBuffer,
                                                            PULONG retBufferLength,
                                                            PNTSTATUS status
                                                        ) = 0;
        virtual NTSTATUS NTAPI LsaConnectUntrusted(PHANDLE lsaHandle) = 0;
        virtual NTSTATUS NTAPI LsaDeregisterLogonProcess(HANDLE lsaHandle) = 0;
        virtual NTSTATUS NTAPI LsaFreeReturnBuffer(PVOID buffer) = 0;
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
                                        ) = 0;
        virtual NTSTATUS NTAPI LsaLookupAuthenticationPackage(HANDLE lsaHandle, PLSA_STRING packageName, PULONG authenticationPackageId) = 0;
    };

    typedef std::unique_ptr<ISecur32Wrapper> Secur32WrapperPtr;
}

#endif // !I_SECUR32_WRAPPER
