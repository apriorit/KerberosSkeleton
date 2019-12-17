#include <iostream>
#include "SpnManager.h"
#include "SystemApiWrapper.h"
#include "Secur32Wrapper.h"
#include "KerberosAuthenticator.h"
#include "KerberosException.h"

using namespace KerberosServer;

typedef std::unique_ptr<AKerbAuthenticator> KerbAuthenticatorPtr;
typedef std::unique_ptr<void, decltype(&::CloseHandle)> TokenHandlePtr;

void ShowHelp()
{
    std::cout << "Run the app with one of following options" << std::endl;
    std::cout << "    [-spn] - to obtain SPN for ClientApp" << std::endl;
    std::cout << "    [-h] - to show this help" << std::endl;
    std::cout << "    [-auth] - to perform authentication via using both the krbtgtTicket.bin and the serviceTicket.bin that were placed near this app" << std::endl;
}

void LoadFileToVector(HANDLE hFile, std::vector<unsigned char>& data)
{
    if (hFile == INVALID_HANDLE_VALUE)
        throw std::logic_error("LoadFileToVector : can't open file ");
    DWORD fileSize = ::GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE)
        throw std::logic_error("LoadFileToVector : can't get file size for ");

    if (!fileSize)
        return;

    data.resize(fileSize);
    DWORD bytesRead = 0;

    if (!ReadFile(hFile, &data[0], fileSize, &bytesRead, 0) || bytesRead != fileSize)
        throw std::logic_error("LoadFileToVector : can't read file ");
}

void LoadFileToVectorStr(const std::string& fileName, std::vector<unsigned char>& data)
{
    HANDLE hFile = ::CreateFileA(fileName.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
        throw std::logic_error("LoadFileToVectorStr : can't open file " + fileName);
    LoadFileToVector(hFile, data);
}

std::string Authenticate()
{
    std::string result("Success completed task of the authentication");
    try
    {
        TicketData serviceTicket, krbtgtTicket;
        LoadFileToVectorStr("serviceTicket.bin", serviceTicket);
        LoadFileToVectorStr("krbtgtTicket.bin", krbtgtTicket);

        Secur32::Secur32WrapperPtr secur32Wrapper(new Secur32::Secur32Wrapper());
        KerbAuthenticatorPtr kerbAuthenticator(new KerberosAuthenticator(std::move(secur32Wrapper)));

        TokenHandlePtr hTokeen(kerbAuthenticator->Authenticate(serviceTicket, krbtgtTicket), &::CloseHandle);

        if (hTokeen.get() == nullptr || hTokeen.get() == INVALID_HANDLE_VALUE)
        {
            result = "Failed task of the authentication \n"
                     "because retrived handle is invalid";
        }
    }
    catch (const KerbException::KerberosException&)
    {
        result = "Failed task of the authentication";
    }
    catch (const std::logic_error&)
    {
        result = "Failed task of the authentication, \n"
            "because both the krbtgtTicket.bin and the serviceTicket.bin \n"
            "must be placed near this app";
    }

    return result;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Incorrect input parameters" << std::endl;
        ShowHelp();
        return 0;
    }
    std::string inputParameter = argv[1];
    
    if (inputParameter == "-h")
    {
        ShowHelp();
    }
    else if (inputParameter == "-spn")
    {
        int failingByReason = 0;
        SystemApiWrapperPtr systemApiWrapper(new SystemApiWrapper());
        SpnManagerPtr spnManager(new SpnManager(std::move(systemApiWrapper)));

        std::wcout << spnManager->CreateSpn() << std::endl;
    }
    else if (inputParameter == "-auth")
    {
        std::cout << Authenticate() << std::endl;
    }
    else
    {
        std::cout << "Unknown input parameter" << std::endl;
        ShowHelp();
    }
    return 0;
}