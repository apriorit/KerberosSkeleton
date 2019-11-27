#ifndef A_SPN_RESPONSE_STRATEGY
#define A_SPN_RESPONSE_STRATEGY

#include "IClientResponse.h"
#include "IResponseStrategy.h"
#include "SpnManager.h"

namespace KerberosServer
{
    class ASpnResponseStrategy : public IResponseStrategy
    {
    public:
        explicit ASpnResponseStrategy(SpnManagerPtr spnManager);

        virtual ClientResponsePtr CreateResponse();
    private:
        virtual ClientResponsePtr PackSpnToResponse(const std::wstring& spnData) = 0;

    private:
        SpnManagerPtr m_spnManager;
    };
}


#endif //A_SPN_RESPONSE_STRATEGY
