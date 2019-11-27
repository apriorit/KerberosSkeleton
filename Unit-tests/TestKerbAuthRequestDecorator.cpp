#include "gtest/gtest.h"
#include "SpnValidator.h"
#include "KerbAuthRequestDecorator.h"
#include "Mock/MockSecur32Wrapper.h"
#include "Mock/MockServerRequest.h"
#include "KerberosException.h"
#include <memory>

using namespace KerberosClient;
using namespace Mock;

typedef std::unique_ptr<KerbAuthRequestDecorator> KerbAuthRequestDecoratorPtr;

TEST(KerbAuthRequestDecorator, success)
{
    int functionToFailing = 0;
    Secur32::Secur32WrapperPtr secur32Wrapper(new MockSecur32Wrapper(functionToFailing));
    ServerRequestPtr spnRequest(new MockServerRequest(Mock::RequestType::SPN_Request));
    SpnValidatorPtr spnValidator(new SpnValidator());
    ServerResponsePtr authResponse;
    KerbAuthRequestDecoratorPtr kerbAuthenticator;

    ASSERT_NO_THROW(kerbAuthenticator.reset(new KerbAuthRequestDecorator(std::move(spnRequest), std::move(secur32Wrapper), std::move(spnValidator))));
    ASSERT_NO_THROW(authResponse = kerbAuthenticator->SendRequest());

    ASSERT_EQ(authResponse->GetStringDataFromResponse(L"auth"), L"Some authentication response");
}

TEST(KerbAuthRequestDecorator, invalid_SpnData)
{
    int functionToFailing = 0;
    Secur32::Secur32WrapperPtr secur32Wrapper(new MockSecur32Wrapper(functionToFailing));
    ServerRequestPtr spnRequest(new MockServerRequest(Mock::RequestType::InvalidSPN_Request));
    SpnValidatorPtr spnValidator(new SpnValidator());
    ServerResponsePtr authResponse;
    KerbAuthRequestDecoratorPtr kerbAuthenticator;

    ASSERT_NO_THROW(kerbAuthenticator.reset(new KerbAuthRequestDecorator(std::move(spnRequest), std::move(secur32Wrapper), std::move(spnValidator))));
    ASSERT_THROW(authResponse = kerbAuthenticator->SendRequest(), KerbException::KerberosException);
}

TEST(KerbAuthRequestDecorator, invalid_ThrowExeptionOnLsaConnectUntrusted)
{
    int functionsToFailing = 0;
    functionsToFailing |= Secur32WrapperFailingReason::LsaConnectUntrusted;

    Secur32::Secur32WrapperPtr secur32Wrapper(new MockSecur32Wrapper(functionsToFailing));
    ServerRequestPtr spnRequest(new MockServerRequest(Mock::RequestType::SPN_Request));
    SpnValidatorPtr spnValidator(new SpnValidator());
    ServerResponsePtr authResponse;
    KerbAuthRequestDecoratorPtr kerbAuthenticator;

    ASSERT_NO_THROW(kerbAuthenticator.reset(new KerbAuthRequestDecorator(std::move(spnRequest), std::move(secur32Wrapper), std::move(spnValidator))));
    ASSERT_THROW(authResponse = kerbAuthenticator->SendRequest(), KerbException::KerberosException);
}