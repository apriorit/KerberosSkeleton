#pragma once

#include <exception>
#include <string>

namespace KerbException
{
    class KerberosException : public std::exception
    {
    public:
        enum ErrorType {
            NOT_IMPLEMENTED,
            FAILED_LSACONNECTUNTRUSTED,
            FAILED_LSALOOKUPAUTHENTICATIONPACKAGE,
            FAILED_LSACALLAUTHENTICATIONPACKAGE,
            FAILED_LSALOGONUSER,
            FAILED_LOADINDSECUR32,
            FAILED_LOADINDSECUR32FUNCTIONS,
            INVALID_PROTOCOLSTATUS,
            INVALID_RETURNBUFFER,
            INVALID_SPN_DATA,
            INVALID_SECUR32WRAPPER_INSATNCE
        };

        explicit KerberosException(ErrorType errorType, const std::string &s) : m_errorType(errorType), m_what(s) {}
        virtual ~KerberosException() throw() {}
        const char *what() const throw() { return (m_what.c_str()); }
        const std::string &GetWhat() const { return m_what; }
        void SetWhat(const std::string &s) { m_what = s; }
        ErrorType GetErrorType() const { return m_errorType; }
        void SetErrorType(ErrorType errorType) { m_errorType = errorType; }

    private:
        ErrorType m_errorType;
        std::string m_what;
    };
}

