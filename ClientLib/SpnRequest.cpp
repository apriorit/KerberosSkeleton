#include "SpnRequest.h"
#include "KerberosException.h"

using namespace KerberosClient;
using namespace KerbException;

ServerResponsePtr SpnRequest::SendRequest()
{
    throw KerberosException(KerberosException::ErrorType::NOT_IMPLEMENTED, "SpnRequest::SendRequest: the function is not implemented yet");
}