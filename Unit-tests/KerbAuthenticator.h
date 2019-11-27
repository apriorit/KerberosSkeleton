#ifndef KERBEROS_AUTHENTICATOR
#define KERBEROS_AUTHENTICATOR

#include "AKerbAuthenticator.h"

namespace KerberosServer
{
    class KerbAuthenticator : public AKerbAuthenticator
    {
    public:
        explicit KerbAuthenticator(Secur32::Secur32WrapperPtr secur32Wrapper);

    private:
        virtual SystemApiWrapperPtr GetSystemApiWrapper()const;
        virtual SECURITY_LOGON_TYPE GetLogonType()const;
        virtual std::string GetOriginName()const;
        virtual std::string GetSourceModuleIdentifier()const;
    };
}

#endif // !KERBEROS_AUTHENTICATOR

