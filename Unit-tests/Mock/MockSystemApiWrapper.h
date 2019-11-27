#ifndef MOCK_SYSTEM_API_WRAPPER
#define MOCK_SYSTEM_API_WRAPPER

#include "ISystemApiWrapper.h"
#include <string>

namespace Mock
{
    enum SystemApiWrapperFailingReason
    {
        EmptyBuffersSize = 1,
        IncorrectValueAtGettingBufferSize = 2,
        IncorrectValueAtGettingBuffer = 4,
        FailedAllocateLocallyUniqueId = 8
    };

    class MockSystemApiWrapper : public KerberosServer::ISystemApiWrapper
    {
    public:
        explicit MockSystemApiWrapper(int failedByReason);

    public:
        virtual BOOL GetComputerNameExW(COMPUTER_NAME_FORMAT nameType, LPWSTR lpBuffer, LPDWORD nSize);
        virtual BOOL AllocateLocallyUniqueId(PLUID Luid);

    private:
        void CopyComputerNameToBuffer(std::wstring computerName, LPWSTR lpBuffer, LPDWORD nSize);

    private:
        const std::wstring m_dnsHostName;
        const std::wstring m_dnsDomain;
        int m_failedByReason;
    };
}
#endif //MOCK_SYSTEM_API_WRAPPER
