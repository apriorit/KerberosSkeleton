#ifndef SPN_RESPONSE_STRATEGY
#define SPN_RESPONSE_STRATEGY

#include "ASpnResponseStrategy.h"

namespace KerberosServer
{
    class SpnResponseStrategy : public ASpnResponseStrategy
    {
    public:
        explicit SpnResponseStrategy(SpnManagerPtr spnManager);

        const std::wstring& GetSpnData()const;

    private:
        virtual ClientResponsePtr PackSpnToResponse(const std::wstring& spnData);

    private:
        SpnManagerPtr m_spnManager;
        std::wstring m_spnData;
    };
}


#endif //SPN_RESPONSE_STRATEGY
