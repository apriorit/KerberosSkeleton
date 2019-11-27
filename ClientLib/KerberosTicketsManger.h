#ifndef KERBEROS_TICKET_MANAGER
#define KERBEROS_TICKET_MANAGER

#include "ISecur32Wrapper.h"
#include <functional>

namespace KerberosClient
{
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
