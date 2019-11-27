#ifndef I_RESPONSE_STRATEGY_DETECTOR
#define I_RESPONSE_STRATEGY_DETECTOR

#include "IResponseStrategy.h"
#include <vector>
#include <memory>

namespace KerberosServer
{
    class IResponseStrategyDetector
    {
    public:
        virtual ~IResponseStrategyDetector() {};
        virtual ResponseStrategyPtr CreateResponseStrategy(const std::vector<char>& requestData) = 0;
    };

    typedef std::unique_ptr<IResponseStrategyDetector> ResponseStrategyDetectorPtr;
}


#endif //I_RESPONSE_STRATEGY_DETECTOR
