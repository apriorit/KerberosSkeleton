#include "ASpnResponseStrategy.h"

using namespace KerberosServer;

ASpnResponseStrategy::ASpnResponseStrategy(SpnManagerPtr spnManager) :
    m_spnManager(std::move(spnManager))
{
}

ClientResponsePtr ASpnResponseStrategy::CreateResponse()
{
    std::wstring spnData = m_spnManager->CreateSpn();
    return PackSpnToResponse(spnData);
}