#ifndef KERB_RETRIVE_TICKET_REQUEST
#define KERB_RETRIVE_TICKET_REQUEST

#include <string>
#include <vector>
#include "ISecur32Wrapper.h"

namespace KerberosClient
{
    class KerbRetrieveTktRequest
    {
    public:
        explicit KerbRetrieveTktRequest(const std::wstring& targetName);

        KERB_RETRIEVE_TKT_REQUEST* GetRetrieveTktRequest();
        ULONG Length()const;
        void SetTicketFlags(ULONG ticketFlags);
        void SetCacheOptions(ULONG cacheOptions);

    private:
        std::wstring m_targetName;
        std::vector<char> m_retrieveTktRequestData;
        unsigned long m_ticketFlags;
        unsigned long m_cacheOptions;
    };
}

#endif // !KERB_RETRIVE_TICKET_REQUEST
