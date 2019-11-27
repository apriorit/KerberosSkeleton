#ifndef MOCK_SPN_REQUEST
#define MOCK_SPN_REQUEST

#include "IServerRequest.h"

namespace Mock
{
    enum RequestType
    {
        SPN_Request,
        InvalidSPN_Request,
        Authentication_Request
    };

    class MockServerRequest : public KerberosClient::IServerRequest
    {
    public:
        explicit MockServerRequest(RequestType requestType);
    public:
        virtual KerberosClient::ServerResponsePtr SendRequest();
    private:
        RequestType m_RequestType;
    };
}

#endif // !MOCK_SPN_REQUEST
