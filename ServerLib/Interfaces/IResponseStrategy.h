#ifndef I_RESPONSE_STRATEGY
#define I_RESPONSE_STRATEGY

#include "IClientResponse.h"
#include <vector>
#include <memory>

namespace KerberosServer
{
    class IResponseStrategy
    {
    public:
        virtual ~IResponseStrategy() {};
        virtual ClientResponsePtr CreateResponse() = 0;
    };

    typedef std::unique_ptr<IResponseStrategy> ResponseStrategyPtr;
}


#endif //I_RESPONSE_STRATEGY
