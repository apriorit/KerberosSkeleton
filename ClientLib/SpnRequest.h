#ifndef SPN_REQUEST
#define SPN_REQUEST

#include "IServerRequest.h"

namespace KerberosClient
{
    class SpnRequest : public IServerRequest
    {
    public:
        virtual ServerResponsePtr SendRequest();
    };
}


#endif //SPN_REQUEST
