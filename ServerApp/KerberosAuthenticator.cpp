#include "KerberosAuthenticator.h"

using namespace KerberosServer;

#include "KerberosAuthenticator.h"
#include "SystemApiWrapper.h"

using namespace KerberosServer;

KerberosAuthenticator::KerberosAuthenticator(Secur32::Secur32WrapperPtr secur32Wrapper) :
    AKerbAuthenticator(std::move(secur32Wrapper))
{
}

SystemApiWrapperPtr KerberosAuthenticator::GetSystemApiWrapper()const
{
    return SystemApiWrapperPtr(new SystemApiWrapper());
}

SECURITY_LOGON_TYPE KerberosAuthenticator::GetLogonType()const
{
    return SECURITY_LOGON_TYPE::Network;
}

std::string KerberosAuthenticator::GetOriginName()const
{
    return "exmln";
}

std::string KerberosAuthenticator::GetSourceModuleIdentifier()const
{
    return "exm";
}