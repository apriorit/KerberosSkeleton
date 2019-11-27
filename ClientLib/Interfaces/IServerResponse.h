#ifndef I_RESPONSE_CLIENT
#define I_RESPONSE_CLIENT

#include <string>
#include <memory>

namespace KerberosClient
{
    class IServerResponse
    {
    public:
        virtual ~IServerResponse() {};
        virtual std::wstring GetStringDataFromResponse(const std::wstring& key) = 0;
    };

    typedef std::unique_ptr<IServerResponse> ServerResponsePtr;
}


#endif //I_RESPONSE_CLIENT
