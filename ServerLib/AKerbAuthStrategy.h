#ifndef A_KERB_AUTH_STRATEGY
#define A_KERB_AUTH_STRATEGY

#include <windows.h>
#include <functional>
#include "IClientResponse.h"
#include "IResponseStrategy.h"
#include "ISecur32Wrapper.h"

namespace KerberosServer
{
    class AKerbAuthenticator;
    typedef std::unique_ptr<AKerbAuthenticator> KerbAuthenticatorPtr;

    class AKerbAuthStrategy : public IResponseStrategy
    {
    public:
        AKerbAuthStrategy(TicketData& serviceTicket, TicketData& krbtgtTicket);

        ClientResponsePtr CreateResponse();

    private:
        virtual ClientResponsePtr PackSpnToResponse(const std::string& responseStr) = 0;
        virtual Secur32::Secur32WrapperPtr GetSecur32Wrapper()const = 0;
        virtual KerbAuthenticatorPtr GetKerbAuthenticator()const = 0;

    private:
        const TicketData& GetServiceTicket()const;
        const TicketData& GetKrbtgtTicket()const;

        typedef std::unique_ptr<void, decltype(&::CloseHandle)> TokenHandlePtr;

    private:
        TicketData m_serviceTicket;
        TicketData m_krbtgtTicket;
        TokenHandlePtr m_hToken;
    };
}


#endif //A_KERB_AUTH_STRATEGY
