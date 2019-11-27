#include "SpnResponseStrategy.h"
#include "ClientResponse.h"

using namespace KerberosServer;

SpnResponseStrategy::SpnResponseStrategy(SpnManagerPtr spnManager) :
    ASpnResponseStrategy(std::move(spnManager))
{
}

ClientResponsePtr SpnResponseStrategy::PackSpnToResponse(const std::wstring& spnData)
{
    m_spnData = spnData;
    return ClientResponsePtr(new ClientResponse());
}

const std::wstring& SpnResponseStrategy::GetSpnData()const
{
    return m_spnData;
}