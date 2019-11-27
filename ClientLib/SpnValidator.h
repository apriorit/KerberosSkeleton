#ifndef SPN_VALIDATOR
#define SPN_VALIDATOR

#include "ISpnValidator.h"

namespace KerberosClient
{
    class SpnValidator : public ISpnValidator
    {
    public:
        virtual bool Validate(const std::wstring& spnData);
    private:
        static const std::string REGEX_STRING_FORMAT;
    };
}


#endif //SPN_VALIDATOR
