#ifndef A_KERBEROS_AUTHENTICATOR
#define A_KERBEROS_AUTHENTICATOR

#include "ISecur32Wrapper.h"
#include "ISystemApiWrapper.h"
#include <vector>
#include <functional>

namespace KerberosServer
{
    class AKerbAuthenticator
    {
    public:
        explicit AKerbAuthenticator(Secur32::Secur32WrapperPtr secur32Wrapper);
        virtual ~AKerbAuthenticator() {};

        HANDLE Authenticate(const TicketData& serviceTicket, const TicketData& krbtgtTicket);

    private:
        typedef std::function<void(HANDLE)> LsaHandleDeleter;
        typedef std::function<void(PKERB_TICKET_PROFILE)> LsaBufferDeleter;
        LsaHandleDeleter GetLsaHandleDeleter()const;
        LsaBufferDeleter GetLsaBufferDeleter()const;

        void InitializeUntrustedConnect();

    private:
        virtual SystemApiWrapperPtr GetSystemApiWrapper()const = 0;
        virtual SECURITY_LOGON_TYPE GetLogonType()const = 0;
        virtual std::string GetOriginName()const = 0;
        virtual std::string GetSourceModuleIdentifier()const = 0;

    private:
        typedef std::unique_ptr<void, LsaHandleDeleter> LsaHandlePtr;

        Secur32::Secur32WrapperPtr m_secur32Wrapper;
        LsaHandlePtr m_hLsa;
        ULONG m_authPkgId;
    };
}

#endif // !A_KERBEROS_AUTHENTICATOR
