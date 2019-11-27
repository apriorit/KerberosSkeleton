#include "gtest/gtest.h"
#include "SpnResponseStrategy.h"
#include "Mock/MockSystemApiWrapper.h"
#include "KerbServerException.h"

using namespace KerberosServer;
using namespace Mock;

TEST(SpnResponseStrategy, success)
{
    int failingByReason = 0;
    SystemApiWrapperPtr systemApiWrapper(new MockSystemApiWrapper(failingByReason));
    SpnManagerPtr spnManager(new SpnManager(std::move(systemApiWrapper)));

    SpnResponseStrategy spnResponseStrategy(std::move(spnManager));

    ClientResponsePtr response;

    ASSERT_NO_THROW(response = spnResponseStrategy.CreateResponse());
    const std::wstring& spnValue = spnResponseStrategy.GetSpnData();
    ASSERT_EQ(spnValue, L"host/DnsHostname.DnsDomain@DnsDomain");
}