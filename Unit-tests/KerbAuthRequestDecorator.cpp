#include "KerbAuthRequestDecorator.h"
#include "Mock/MockServerRequest.h"

using namespace KerberosClient;

KerbAuthRequestDecorator::KerbAuthRequestDecorator(ServerRequestPtr request, Secur32::Secur32WrapperPtr secur32Wrapper, SpnValidatorPtr spnValidator) :
    AKerbAuthRequestDecorator(std::move(request), std::move(secur32Wrapper), std::move(spnValidator))
{
}

ServerRequestPtr KerbAuthRequestDecorator::PackTicketsToRequest(const TicketData&, const TicketData&)
{
    return ServerRequestPtr(new Mock::MockServerRequest(Mock::RequestType::Authentication_Request));
}