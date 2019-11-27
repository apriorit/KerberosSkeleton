#include "LsaLogonUserDataManager.h"
#include "KerbServerException.h"

using namespace KerberosServer;
using namespace KerbException;

LsaLogonUserDataManager::LsaLogonUserDataManager(SystemApiWrapperPtr& systemWrapper, const std::string& origin, const std::string& srcModuleIdentifies) :
    m_systemWrapper(std::move(systemWrapper)),
    m_kerbTicketLogonFlag(0),
    m_origin(origin.begin(), origin.end()),
    m_srcModuleIdentifies(srcModuleIdentifies)
{
    memset(&m_logonId, 0, sizeof(LUID));
    memset(&m_quotaLimits, 0, sizeof(QUOTA_LIMITS));
}

KERB_TICKET_LOGON* LsaLogonUserDataManager::GetKerbTicketLogon(const TicketData& serviceTicket, const TicketData& krbtgtTicket, ULONG& length)
{
    length = sizeof(KERB_TICKET_LOGON) + static_cast<ULONG>(serviceTicket.size()) + static_cast<ULONG>(krbtgtTicket.size());
    m_kerbTicketLogonData.resize(length, 0);

    KERB_TICKET_LOGON* pKerbTicketLogon = reinterpret_cast<KERB_TICKET_LOGON*>(&m_kerbTicketLogonData[0]);

    pKerbTicketLogon->MessageType = KerbTicketLogon;
    pKerbTicketLogon->Flags = m_kerbTicketLogonFlag;
    pKerbTicketLogon->ServiceTicketLength = static_cast<ULONG>(serviceTicket.size());
    pKerbTicketLogon->TicketGrantingTicketLength = static_cast<ULONG>(krbtgtTicket.size());
    pKerbTicketLogon->ServiceTicket = reinterpret_cast<PUCHAR>(pKerbTicketLogon + 1);
    memcpy(pKerbTicketLogon->ServiceTicket, &serviceTicket[0], serviceTicket.size());
    pKerbTicketLogon->TicketGrantingTicket = pKerbTicketLogon->ServiceTicket + serviceTicket.size();
    memcpy(pKerbTicketLogon->TicketGrantingTicket, &krbtgtTicket[0], krbtgtTicket.size());

    return pKerbTicketLogon;
}

void LsaLogonUserDataManager::InitLsaOrigin(LSA_STRING& origin)
{
    origin.Length = static_cast<USHORT>(m_origin.size());
    origin.MaximumLength = static_cast<USHORT>(m_origin.size());
    origin.Buffer = m_origin.data();
}

void LsaLogonUserDataManager::InitTokenSource(TOKEN_SOURCE& srcContext)const
{
    memset(&srcContext, 0, sizeof(TOKEN_SOURCE));

    const char* tmp = m_srcModuleIdentifies.c_str();

    strncpy_s(srcContext.SourceName, TOKEN_SOURCE_LENGTH, m_srcModuleIdentifies.c_str(), TOKEN_SOURCE_LENGTH - 1);

    if (!m_systemWrapper->AllocateLocallyUniqueId(&srcContext.SourceIdentifier))
    {
        DWORD error = ::GetLastError();
        // TO DO: Write to logs the error code;

        throw KerbServerException(KerbServerException::FAILED_INIT_TOKENSOURCE, "LsaLogonUserDataManager::LsaLogonUserDataManager: Failed on allocation the local unique Id");
    }
}

PLUID LsaLogonUserDataManager::GetDefaultLogonId()
{
    return &m_logonId;
}

PQUOTA_LIMITS LsaLogonUserDataManager::GetDefaultQuotaLimits()
{
    return &m_quotaLimits;
}