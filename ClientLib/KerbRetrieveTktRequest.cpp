#include "KerbRetrieveTktRequest.h"
#include <windows.h>

using namespace KerberosClient;

KerbRetrieveTktRequest::KerbRetrieveTktRequest(const std::wstring& targetName) :
    m_targetName(targetName),
    m_retrieveTktRequestData(sizeof(KERB_RETRIEVE_TKT_RESPONSE) + (m_targetName.length() * sizeof(wchar_t)), 0),
    m_ticketFlags(KERB_USE_DEFAULT_TICKET_FLAGS),
    m_cacheOptions(KERB_RETRIEVE_TICKET_DEFAULT)
{
}

KERB_RETRIEVE_TKT_REQUEST* KerbRetrieveTktRequest::GetRetrieveTktRequest()
{
    KERB_RETRIEVE_TKT_REQUEST* ret = NULL;
    ret = reinterpret_cast<KERB_RETRIEVE_TKT_REQUEST*>(&m_retrieveTktRequestData[0]);

    LUID luidLogonId;
    memset(&luidLogonId, 0, sizeof(LUID));

    SecHandle hSec;
    memset(&hSec, 0, sizeof(SecHandle));

    ret->MessageType = KERB_PROTOCOL_MESSAGE_TYPE::KerbRetrieveEncodedTicketMessage;
    ret->LogonId = luidLogonId;

    ret->TargetName.Length = static_cast<USHORT>(m_targetName.length() * sizeof(wchar_t));
    ret->TargetName.MaximumLength = static_cast<USHORT>(m_targetName.length() * sizeof(wchar_t));
    ret->TargetName.Buffer = (wchar_t*)(ret + 1);
    memcpy(ret->TargetName.Buffer, m_targetName.c_str(), ret->TargetName.Length);

    ret->TicketFlags = m_ticketFlags;
    ret->CacheOptions = m_cacheOptions;
    ret->EncryptionType = 0;
    ret->CredentialsHandle = hSec;

    return ret;
}

ULONG KerbRetrieveTktRequest::Length()const
{
    return static_cast<ULONG>(m_retrieveTktRequestData.size());
}

void KerbRetrieveTktRequest::SetTicketFlags(ULONG ticketFlags)
{
    m_ticketFlags = ticketFlags;
}

void KerbRetrieveTktRequest::SetCacheOptions(ULONG cacheOptions)
{
    m_cacheOptions = cacheOptions;
}