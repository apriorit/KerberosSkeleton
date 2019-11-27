#ifndef MOCK_SPN_RESPONSE
#define MOCK_SPN_RESPONSE

#include "IServerResponse.h"

namespace Mock
{
    enum ResponseType
    {
        SPN_Response,
        InvalidSpn_Response,
        Authentication_Response
    };

    class MockServerResponse : public KerberosClient::IServerResponse
    {
    public:
        explicit MockServerResponse(ResponseType responseType);
    public:
        virtual std::wstring GetStringDataFromResponse(const std::wstring& key);
    private:
        ResponseType m_responseType;
    };
}

#endif // !MOCK_SPN_RESPONSE
