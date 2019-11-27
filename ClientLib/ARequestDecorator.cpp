#include "ARequestDecorator.h"
#include "KerberosException.h"

using namespace KerberosClient;

ARequestDecorator::ARequestDecorator(ServerRequestPtr request) :
    m_request(std::move(request))
{
}

IServerRequest* ARequestDecorator::GetRequest()const
{
    return m_request.get();
}
