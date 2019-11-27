#include "SpnManager.h"
#include <vector>
#include "KerbServerException.h"
#include <sstream>

using namespace KerberosServer;
using namespace KerbException;

SpnManager::SpnManager(SystemApiWrapperPtr systemApiWrapper) :
    m_systemApiWrapper(std::move(systemApiWrapper))
{
}

std::wstring SpnManager::CreateSpn()const
{
    std::wstringstream result;
    result << L"host/";

    result << GetNameByType(ComputerNameDnsHostname);
    result << L".";
    result << GetNameByType(ComputerNameDnsDomain);
    result << L"@";
    result << GetNameByType(ComputerNameDnsDomain);

    return result.str();
}

std::wstring SpnManager::GetNameByType(COMPUTER_NAME_FORMAT nameType)const
{
    DWORD bufferSize = 0;
    BOOL isOk = m_systemApiWrapper->GetComputerNameExW(nameType, NULL, &bufferSize);

    if (isOk || bufferSize == 0)
    {
        throw KerbServerException(KerbServerException::FAILED_GETCOMPUTERNAMEEXW_BUFFER_SIZE, "SpnManager::GetNameByType: Failed to get buffer size");
    }

    std::vector<wchar_t> buffer(static_cast<size_t>(bufferSize), 0);

    isOk = m_systemApiWrapper->GetComputerNameExW(nameType, &buffer.at(0), &bufferSize);
    if (!isOk)
    {
        throw KerbServerException(KerbServerException::FAILED_GETCOMPUTERNAMEEXW_BUFFER, "SpnManager::GetNameByType: Failed to get bufffer content");
    }

    std::wstring result(buffer.begin(), buffer.begin() + bufferSize);
    return result;
}