#include "MockServerRequest.h"
#include "MockServerResponse.h"

using namespace Mock;

MockServerRequest::MockServerRequest(RequestType requestType) : m_RequestType(requestType)
{
}

KerberosClient::ServerResponsePtr MockServerRequest::SendRequest()
{
    if (m_RequestType == RequestType::SPN_Request)
    {
        return KerberosClient::ServerResponsePtr(new MockServerResponse(ResponseType::SPN_Response));
    }

    if (m_RequestType == RequestType::InvalidSPN_Request)
    {
        return KerberosClient::ServerResponsePtr(new MockServerResponse(ResponseType::InvalidSpn_Response));
    }

    if (m_RequestType == RequestType::Authentication_Request)
    {
        return KerberosClient::ServerResponsePtr(new MockServerResponse(ResponseType::Authentication_Response));
    }

    return KerberosClient::ServerResponsePtr();
}