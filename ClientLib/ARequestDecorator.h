#ifndef A_REQUEST_DECORATOR
#define A_REQUEST_DECORATOR

#include "IServerRequest.h"
#include <vector>

namespace KerberosClient
{
    class ARequestDecorator : public IServerRequest
    {
    public:
        explicit ARequestDecorator(ServerRequestPtr request);

    public:
        virtual ServerResponsePtr SendRequest() = 0;

    protected:
        IServerRequest* GetRequest()const;

    private:
        ServerRequestPtr m_request;
    };
}


#endif //SPN_REQUEST
