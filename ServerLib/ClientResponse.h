#ifndef RESPONSE_SERVER
#define RESPONSE_SERVER

#include "IClientResponse.h"

namespace KerberosServer
{
    class ClientResponse : public IClientResponse
    {
    public:
        virtual HRESULT SendResponse();
    };
}

#endif //RESPONSE_SERVER
