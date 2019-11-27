#ifndef SPN_MANAGER_SERVER
#define SPN_MANAGER_SERVER

#include <string>
#include "ISystemApiWrapper.h"

namespace KerberosServer
{
    class SpnManager
    {
    public:
        explicit SpnManager(SystemApiWrapperPtr systemApiWrapper);

    public:
        std::wstring CreateSpn()const;

    private:
        std::wstring GetNameByType(COMPUTER_NAME_FORMAT nameType)const;

    private:
        SystemApiWrapperPtr m_systemApiWrapper;
    };

    typedef std::unique_ptr<SpnManager> SpnManagerPtr;
}


#endif //SPN_MANAGER_SERVER
