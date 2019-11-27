#ifndef I_REQUEST
#define I_REQUEST

#include "IServerResponse.h"

namespace KerberosClient
{
    class IServerRequest
    {
    public:
        virtual ~IServerRequest() {};
        virtual ServerResponsePtr SendRequest() = 0;
    };

    typedef std::unique_ptr<IServerRequest> ServerRequestPtr;
}


#endif //I_REQUEST
