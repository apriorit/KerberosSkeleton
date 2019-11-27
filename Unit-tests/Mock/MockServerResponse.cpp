#include "MockServerResponse.h"

using namespace Mock;

MockServerResponse::MockServerResponse(ResponseType responseType) :m_responseType(responseType)
{
}

std::wstring MockServerResponse::GetStringDataFromResponse(const std::wstring& /*key*/)
{
    if (m_responseType == ResponseType::SPN_Response)
    {
        return L"host/DnsHostname.DnsDomain@DnsDomain";
    }
    if (m_responseType == ResponseType::InvalidSpn_Response)
    {
        return L"host/<InvalidDnsHostname>.DnsDomain@DnsDomain";
    }
    if (m_responseType == ResponseType::Authentication_Response)
    {
        return L"Some authentication response";
    }

    return L"Unknown response type";
}