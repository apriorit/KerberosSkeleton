#include "KerbAuthenticator.h"
#include "Mock/MockSystemApiWrapper.h"

using namespace KerberosServer;

KerbAuthenticator::KerbAuthenticator(Secur32::Secur32WrapperPtr secur32Wrapper) :
    AKerbAuthenticator(std::move(secur32Wrapper))
{
}

SystemApiWrapperPtr KerbAuthenticator::GetSystemApiWrapper()const
{
    return SystemApiWrapperPtr(new Mock::MockSystemApiWrapper(0));
}

SECURITY_LOGON_TYPE KerbAuthenticator::GetLogonType()const
{
    return SECURITY_LOGON_TYPE::Network;
}

std::string KerbAuthenticator::GetOriginName()const
{
    return "exmln";
}

std::string KerbAuthenticator::GetSourceModuleIdentifier()const
{
    return "exm";
}