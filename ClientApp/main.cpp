#include <iostream>
#include "SpnValidator.h"
#include "KerberosTicketsManger.h"
#include "KerberosException.h"
#include "Secur32Wrapper.h"
#include <fstream>
#include <stdexcept>

using namespace KerberosClient;

typedef std::unique_ptr<KerberosTicketsManger> KerberosTicketsMangerPtr;
typedef std::shared_ptr<Secur32::ISecur32Wrapper> Secur32WrapperPtr;

void ShowHelp()
{
    std::cout << "Run the app with one of following options" << std::endl;
    std::cout << "    [-spn <SPN value>] - This command launches obtaining Kerberos ticket for the ServerApp" << std::endl;
    std::cout << "                         <SPN value> is the service principal name (SPN) that was obtained from ServiceApp before." << std::endl;
    std::cout << "    [-h] - This command showes this help" << std::endl;
}

void WriteDataToFile(const std::string& fileName, const TicketData& ticketData)
{
    if (!fileName.empty())
    {
        std::ofstream ticketFile;
        ticketFile.open(fileName.c_str(), std::ios::out | std::ios::binary);
        ticketFile.write((char*)(&ticketData.at(0)), ticketData.size());
        ticketFile.close();
    }
}

std::wstring utf8toUtf16(const std::string & inputStr)
{
    if (inputStr.empty())
        return std::wstring();

    size_t charsNeeded = ::MultiByteToWideChar(CP_UTF8, 0, inputStr.data(), (int)inputStr.size(), NULL, 0);
    if (charsNeeded == 0)
        throw std::runtime_error("Failed converting UTF-8 string to UTF-16");

    std::vector<wchar_t> buffer(charsNeeded);
    int charsConverted = ::MultiByteToWideChar(CP_UTF8, 0, inputStr.data(), (int)inputStr.size(), &buffer[0], buffer.size());
    if (charsConverted == 0)
        throw std::runtime_error("Failed converting UTF-8 string to UTF-16");

    return std::wstring(&buffer[0], charsConverted);
}

std::string CreateTickets(const std::wstring& servicePrincipalName)
{
    std::string result("Success completed task of creating the Kerberos tickets");
    try
    {
        TicketData serviceTicket, krbtgtTicket;
        Secur32WrapperPtr secur32Wrapper(new Secur32::Secur32Wrapper());
        SpnValidatorPtr spnValidator(new SpnValidator());
        const ULONG ticketFlags = KerberosTicketOptions::Forwardable | KerberosTicketOptions::Forwarded | KerberosTicketOptions::Renewable | KerberosTicketOptions::Pre_authent;
        static const std::wstring krbtgtName = L"krbtgt";

        const KerberosTicketsMangerPtr kerbTicketsManger(new KerberosTicketsManger(secur32Wrapper));
        kerbTicketsManger->SetTicketFlags(ticketFlags);
        kerbTicketsManger->RequestTicketFromSystem(serviceTicket, servicePrincipalName);
        kerbTicketsManger->SetCacheOptions(KERB_RETRIEVE_TICKET_AS_KERB_CRED);
        kerbTicketsManger->RequestTicketFromSystem(krbtgtTicket, krbtgtName);

        WriteDataToFile("serviceTicket.bin", serviceTicket);
        WriteDataToFile("krbtgtTicket.bin", krbtgtTicket);
    }
    catch (const KerbException::KerberosException&)
    {
        result = "Failed task of creating the Kerberos tickets";
    }

    return result;
}


int main(int argc, char* argv[])
{
    if (argc != 2 && argc != 3)
    {
        std::cout << "Incorrect input parameters" << std::endl;
        ShowHelp();
        return 0;
    }

    std::string inputParameter = argv[1];

    if (inputParameter == "-h" && argc == 2)
    {
        ShowHelp();
    }
    else if (inputParameter == "-spn" && argc == 3)
    {
        std::wstring servicePrincipalName = utf8toUtf16(argv[2]);
        
        std::cout << CreateTickets(servicePrincipalName) << std::endl;
    }
    else
    {
        std::cout << "Unknown input parameter" << std::endl;
        ShowHelp();
    }

    return 0;
}