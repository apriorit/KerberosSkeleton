#include "ClientResponse.h"
#include "KerberosException.h"

using namespace KerberosServer;
using namespace KerbException;

HRESULT ClientResponse::SendResponse()
{
    throw KerberosException(KerberosException::ErrorType::NOT_IMPLEMENTED, "ClientResponse::SendResponse: the function is not implemented yet");
}