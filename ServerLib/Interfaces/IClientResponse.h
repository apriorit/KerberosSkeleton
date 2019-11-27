#ifndef I_RESPONSE_SERVER
#define I_RESPONSE_SERVER

#include <winerror.h>
#include <memory>

namespace KerberosServer
{
    class IClientResponse
    {
    public:
        virtual ~IClientResponse() {};
        virtual HRESULT SendResponse() = 0;
    };

    typedef std::unique_ptr<IClientResponse> ClientResponsePtr;
}


#endif //I_RESPONSE_CLIENT
