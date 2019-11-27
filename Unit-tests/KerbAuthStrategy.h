#ifndef KERB_AUTH_STRATEGY
#define KERB_AUTH_STRATEGY

#include "AKerbAuthStrategy.h"

namespace KerberosServer
{
    enum KerbAuthStrategyFailingReason
    {
        Failed_LsaLogonUser = 1,
        Failed_CreatingSecur32Wrapper = 2
    };

    class KerbAuthStrategy : public AKerbAuthStrategy
    {
    public:
        KerbAuthStrategy(TicketData& serviceTicket, TicketData& krbtgtTicket, int failingReason);

    public:
        const std::string& GetAuthenticationResult()const;

    private:
        virtual ClientResponsePtr PackSpnToResponse(const std::string& responseStr);
        virtual Secur32::Secur32WrapperPtr GetSecur32Wrapper()const;
        virtual KerbAuthenticatorPtr GetKerbAuthenticator()const;

    private:
        std::string m_authenticationResult;
        int m_failingReason;
    };
}


#endif //KERB_AUTH_STRATEGY

