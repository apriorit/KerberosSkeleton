#ifndef I_SPN_VALIDATOR
#define I_SPN_VALIDATOR

#include <string>
#include <memory>

namespace KerberosClient
{
    class ISpnValidator
    {
    public:
        virtual ~ISpnValidator() {};
        virtual bool Validate(const std::wstring&) = 0;
    };

    typedef std::unique_ptr<ISpnValidator> SpnValidatorPtr;
}


#endif //I_SPN_VALIDATOR

