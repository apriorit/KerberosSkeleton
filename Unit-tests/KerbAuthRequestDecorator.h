#ifndef KERB_AUTH_REQUEST_DECORATOR
#define KERB_AUTH_REQUEST_DECORATOR

#include "AKerbAuthRequestDecorator.h"

namespace KerberosClient
{
    class KerbAuthRequestDecorator : public AKerbAuthRequestDecorator
    {
    public:
        KerbAuthRequestDecorator(ServerRequestPtr request, Secur32::Secur32WrapperPtr secur32Wrapper, SpnValidatorPtr spnValidator);

    private:
        virtual ServerRequestPtr PackTicketsToRequest(const TicketData&, const TicketData&);
    };
}

#endif // !KERB_AUTH_REQUEST_DECORATOR
