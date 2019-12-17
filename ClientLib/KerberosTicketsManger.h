#ifndef KERBEROS_TICKET_MANAGER
#define KERBEROS_TICKET_MANAGER

#include "ISecur32Wrapper.h"
#include <functional>

namespace KerberosClient
{
    enum KerberosTicketOptions
    {
        Validate = 1,
        Renew = 2,
        Enc_tkt_in_skey = 8,
        Renewable_ok = 16,
        Disable_transited_check = 32,
        Name_canonicalize = 65536,
        Request_anonymous = 131072,
        Ok_as_delegate = 262144,
        Transited_policy_checked = 524288,
        Opt_hardware_auth = 1048576,
        Pre_authent = 2097152,
        Initial = 4194304,
        Renewable = 8388608,
        Invalid = 16777216,
        Postdated = 33554432,
        Allow_postdate = 67108864,
        Proxy = 134217728,
        Proxiable = 268435456,
        Forwarded = 536870912,
        Forwardable = 1073741824,
        Reserved = 2147483648,
    };

    class KerberosTicketsManger
    {
    public:
        explicit KerberosTicketsManger(const std::shared_ptr<Secur32::ISecur32Wrapper>& secur32Wrapper);

        void RequestTicketFromSystem(TicketData& vecTicket, const std::wstring& tgtName)const;
        void SetTicketFlags(ULONG ticketFlags);
        void SetCacheOptions(ULONG cacheOptions);

    private:
        typedef std::function<void(HANDLE)> LsaHandleDeleter;
        typedef std::function<void(KERB_RETRIEVE_TKT_RESPONSE*)> LsaBufferDeleter;

        void InitializeUntrustedConnect();
        LsaHandleDeleter GetLsaHandleDeleter()const;
        LsaBufferDeleter GetLsaBufferDeleter()const;

    private:
        typedef std::unique_ptr<void, LsaHandleDeleter> LsaHandlePtr;
        typedef std::shared_ptr<Secur32::ISecur32Wrapper> Secur32WrapperPtr;

        Secur32WrapperPtr m_secur32Wrapper;
        LsaHandlePtr m_hLsa;
        ULONG m_authPkgId;
        unsigned long m_ticketFlags;
        unsigned long m_cacheOptions;

    private:
        static const unsigned long DEFAULT_AUTH_PKG_ID;
    };
}

#endif // !KERBEROS_TICKET_MANAGER
