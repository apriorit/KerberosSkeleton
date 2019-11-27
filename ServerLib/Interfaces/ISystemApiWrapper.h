#ifndef I_SYSTEM_API_WRAPPER
#define I_SYSTEM_API_WRAPPER

#include <windows.h>
#include <memory>

namespace KerberosServer
{
    class ISystemApiWrapper
    {
    public:
        virtual ~ISystemApiWrapper() {};
        virtual BOOL GetComputerNameExW(COMPUTER_NAME_FORMAT nameType, LPWSTR lpBuffer, LPDWORD nSize) = 0;
        virtual BOOL AllocateLocallyUniqueId(PLUID Luid) = 0;
    };

    typedef std::unique_ptr<ISystemApiWrapper> SystemApiWrapperPtr;
}


#endif //I_SYSTEM_API_WRAPPER
