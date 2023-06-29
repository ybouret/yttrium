//! \file

#ifndef Y_Exception_Included
#define Y_Exception_Included 1

#include "y/config/starting.hpp"
#include "y/check/printf.hpp"
#include <exception>

namespace Yttrium
{

    class Exception : public std::exception
    {
    public:
        static const size_t BaseSize = 128;
        static const size_t TextSize = BaseSize-sizeof(std::exception);

        explicit Exception()                    noexcept;
        explicit Exception(const char *fmt,...) noexcept Y_PRINTF_API;
        virtual ~Exception() noexcept;
        Exception(const Exception &) noexcept;

        virtual const char *what() const noexcept; //!< defaut: "Exception"
        virtual const char *when() const noexcept; //!< formated story

        Exception & operator<< (const char *) noexcept;
        Exception & operator<< (const char  ) noexcept;
        Exception & add(const char *fmt,...) noexcept Y_PRINTF_API;

        void display() const;

    protected:
        void format(const char *fmt, void *ptr) noexcept;

    private:
        Y_DISABLE_ASSIGN(Exception);
        void reset() noexcept;
        char story[TextSize];
    };

}

#endif

