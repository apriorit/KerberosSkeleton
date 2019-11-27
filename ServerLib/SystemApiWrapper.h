#ifndef SYSTEM_API_WRAPPER
#define SYSTEM_API_WRAPPER

#include "ISystemApiWrapper.h"

namespace KerberosServer
{
    class SystemApiWrapper : public ISystemApiWrapper
    {
    public:
        virtual BOOL GetComputerNameExW(COMPUTER_NAME_FORMAT nameType, LPWSTR lpBuffer, LPDWORD nSize);
        virtual BOOL AllocateLocallyUniqueId(PLUID Luid);
    };
}


#endif //SYSTEM_API_WRAPPER
