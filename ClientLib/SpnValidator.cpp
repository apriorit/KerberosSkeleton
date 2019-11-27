#include "SpnValidator.h"
#include <regex>

using namespace KerberosClient;

const std::string SpnValidator::REGEX_STRING_FORMAT = "^(host\\/)([0-9A-Za-z]{1}[-0-9A-z\\.]{1,}[0-9A-Za-z]{1})\\.([0-9A-Za-z]{1}[-0-9A-z\\.]{1,}[0-9A-Za-z]{1})@([0-9A-Za-z]{1}[-0-9A-z\\.]{1,}[0-9A-Za-z]{1})?$";

bool SpnValidator::Validate(const std::wstring& spnData)
{
    std::regex format(REGEX_STRING_FORMAT.c_str());
    std::cmatch result;
    std::string convertedSPN(spnData.begin(), spnData.end());

    return std::regex_match(convertedSPN.c_str(), result, format);
}