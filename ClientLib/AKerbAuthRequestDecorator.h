#ifndef A_KERB_AUTH_REQUEST_DECORATOR
#define A_KERB_AUTH_REQUEST_DECORATOR

#include "ISpnValidator.h"
#include "ISecur32Wrapper.h"
#include "ARequestDecorator.h"

namespace KerberosClient
{
    class AKerbAuthRequestDecorator : public ARequestDecorator
    {
    public:
        AKerbAuthRequestDecorator(ServerRequestPtr request, Secur32::Secur32WrapperPtr secur32Wrapper, SpnValidatorPtr spnValidator);

        virtual ServerResponsePtr SendRequest();

    private:
        virtual ServerRequestPtr PackTicketsToRequest(const TicketData&, const TicketData&) = 0;

    private:
        typedef std::shared_ptr<Secur32::ISecur32Wrapper> Secur32WrapperPtr;

        SpnValidatorPtr m_spnValidator;
        Secur32WrapperPtr m_secur32Wrapper;
    };
}

#endif // !A_KERB_AUTH_REQUEST_DECORATOR
