#ifndef LSA_LOGON_USER_DATA_MANAGER
#define LSA_LOGON_USER_DATA_MANAGER

#include <string>
#include <vector>
#include "ISecur32Wrapper.h"
#include "ISystemApiWrapper.h"

namespace KerberosServer
{
    class LsaLogonUserDataManager
    {
    public:
        LsaLogonUserDataManager(SystemApiWrapperPtr& systemWrapper, const std::string& origin, const std::string& srcModuleIdentifies);

        KERB_TICKET_LOGON* GetKerbTicketLogon(const TicketData& serviceTicket, const TicketData& krbtgtTicket, ULONG& length);
        void InitLsaOrigin(LSA_STRING& origin);
        void InitTokenSource(TOKEN_SOURCE& srcContext)const;
        PLUID GetDefaultLogonId();
        PQUOTA_LIMITS GetDefaultQuotaLimits();

    private:
        int m_kerbTicketLogonFlag;
        std::vector<char> m_kerbTicketLogonData;
        std::vector<char> m_origin;
        std::string m_srcModuleIdentifies;
        LUID m_logonId;
        QUOTA_LIMITS m_quotaLimits;
        SystemApiWrapperPtr m_systemWrapper;
    };

    typedef std::unique_ptr<LsaLogonUserDataManager> LsaLogonUserDataManagerPtr;
}

#endif // !LSA_LOGON_USER_DATA_MANAGER
