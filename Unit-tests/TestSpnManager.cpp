#include "gtest/gtest.h"
#include "SpnManager.h"
#include "Mock/MockSystemApiWrapper.h"
#include "KerbServerException.h"

using namespace KerberosServer;
using namespace Mock;

TEST(SpnManager, success)
{
    int failingByReason = 0;
    SystemApiWrapperPtr systemApiWrapper(new MockSystemApiWrapper(failingByReason));
    SpnManagerPtr spnManager(new SpnManager(std::move(systemApiWrapper)));

    std::wstring spnValue;

    ASSERT_NO_THROW(spnValue = spnManager->CreateSpn());
    ASSERT_EQ(spnValue, L"host/DnsHostname.DnsDomain@DnsDomain");
}

TEST(SpnManager, failed_OnRetrievingBufferSize)
{
    int failingByReason = SystemApiWrapperFailingReason::EmptyBuffersSize;
    SystemApiWrapperPtr systemApiWrapper(new MockSystemApiWrapper(failingByReason));
    SpnManagerPtr spnManager(new SpnManager(std::move(systemApiWrapper)));

    std::wstring spnValue;

    ASSERT_THROW(spnValue = spnManager->CreateSpn(), KerbException::KerbServerException);
}

TEST(SpnManager, failed_OnRetrievingBufferSize2)
{
    int failingByReason = SystemApiWrapperFailingReason::IncorrectValueAtGettingBufferSize;
    SystemApiWrapperPtr systemApiWrapper(new MockSystemApiWrapper(failingByReason));
    SpnManagerPtr spnManager(new SpnManager(std::move(systemApiWrapper)));

    std::wstring spnValue;

    ASSERT_THROW(spnValue = spnManager->CreateSpn(), KerbException::KerbServerException);
}

TEST(SpnManager, failed_OnRetrievingBuffer)
{
    int failingByReason = SystemApiWrapperFailingReason::IncorrectValueAtGettingBuffer;
    SystemApiWrapperPtr systemApiWrapper(new MockSystemApiWrapper(failingByReason));
    SpnManagerPtr spnManager(new SpnManager(std::move(systemApiWrapper)));

    std::wstring spnValue;

    ASSERT_THROW(spnValue = spnManager->CreateSpn(), KerbException::KerbServerException);
}