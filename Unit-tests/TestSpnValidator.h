#ifndef TEST_SPN_VALIDATOR
#define TEST_SPN_VALIDATOR

#include "gtest/gtest.h"
#include "SpnValidator.h"

class TestSpnValidator
    : public testing::Test
{
protected:
    virtual void SetUp();

    virtual void TearDown();

    KerberosClient::SpnValidatorPtr spnValidator;
};

#endif // TEST_SPN_VALIDATOR