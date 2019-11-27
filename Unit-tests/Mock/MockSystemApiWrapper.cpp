#include "MockSystemApiWrapper.h"

using namespace Mock;

MockSystemApiWrapper::MockSystemApiWrapper(int failedByReason) :
    m_dnsHostName(L"DnsHostname"),
    m_dnsDomain(L"DnsDomain"),
    m_failedByReason(failedByReason)
{
}

BOOL MockSystemApiWrapper::GetComputerNameExW(COMPUTER_NAME_FORMAT nameType, LPWSTR lpBuffer, LPDWORD nSize)
{
    if (lpBuffer == NULL && (m_failedByReason & EmptyBuffersSize) > 0)
    {
        *nSize = 0;
        return FALSE;
    }
    else if (lpBuffer == NULL && (m_failedByReason & IncorrectValueAtGettingBufferSize) > 0)
    {
        *nSize = 1;
        return TRUE;
    }
    else if (lpBuffer != NULL && (m_failedByReason & IncorrectValueAtGettingBuffer) > 0)
    {
        *nSize = 0;
        return FALSE;
    }
    else
    {
        if (nameType == ComputerNameDnsHostname)
        {
            CopyComputerNameToBuffer(m_dnsHostName, lpBuffer, nSize);
        }
        else if (nameType == ComputerNameDnsDomain)
        {
            CopyComputerNameToBuffer(m_dnsDomain, lpBuffer, nSize);
        }
        else
        {
            return FALSE;
        }
    }

    return lpBuffer != NULL;
}

BOOL MockSystemApiWrapper::AllocateLocallyUniqueId(PLUID Luid)
{
    return (m_failedByReason & FailedAllocateLocallyUniqueId) == 0;
}

void MockSystemApiWrapper::CopyComputerNameToBuffer(std::wstring computerName, LPWSTR lpBuffer, LPDWORD nSize)
{
    if (lpBuffer != NULL)
    {
        int minSize = computerName.size() < *nSize ? static_cast<int>(computerName.size()) : *nSize;
        memcpy(lpBuffer, &computerName.at(0), minSize * sizeof(wchar_t));
        *nSize = static_cast<DWORD>(minSize);
    }
    else
    {
        *nSize = static_cast<DWORD>(computerName.size());
    }
}