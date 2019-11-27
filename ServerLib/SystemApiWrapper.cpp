#include "SystemApiWrapper.h"

namespace KerberosServer
{
    BOOL SystemApiWrapper::GetComputerNameExW(COMPUTER_NAME_FORMAT nameType, LPWSTR lpBuffer, LPDWORD nSize)
    {
        return ::GetComputerNameExW(nameType, lpBuffer, nSize);
    }

    BOOL SystemApiWrapper::AllocateLocallyUniqueId(PLUID Luid)
    {
        return ::AllocateLocallyUniqueId(Luid);
    }
}