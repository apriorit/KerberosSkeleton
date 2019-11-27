#pragma once

#include <exception>
#include <string>

namespace KerbException
{
    class KerbServerException : public std::exception
    {
    public:
        enum ErrorType {
            NOT_IMPLEMENTED,
            FAILED_GETCOMPUTERNAMEEXW_BUFFER_SIZE,
            FAILED_GETCOMPUTERNAMEEXW_BUFFER,
            FAILED_INIT_TOKENSOURCE
        };

        explicit KerbServerException(ErrorType errorType, const std::string &s) : m_errorType(errorType), m_what(s) {}
        virtual ~KerbServerException() throw() {}
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
