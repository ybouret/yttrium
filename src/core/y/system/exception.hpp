
//! \file

#ifndef Y_System_Exception_Included
#define Y_System_Exception_Included 1

#include "y/exception.hpp"

namespace Yttrium
{

    namespace Specific
    {
        //______________________________________________________________________
        //
        //
        //! Exception with strerror(err)
        //
        //______________________________________________________________________
        class Exception : public Yttrium::Exception
        {
        public:
            static const size_t TitleSize = ClassSize;

            explicit Exception(const char *header, const char *fmt,...) noexcept Y_PRINTF_CHECK(3,4);
            virtual ~Exception() noexcept;
            Exception(const Exception &) noexcept;

            virtual const char *what() const noexcept; //!< title

        private:
            Y_DISABLE_ASSIGN(Exception);
            char title[TitleSize];
        };
    }

    namespace Libc
    {
        //______________________________________________________________________
        //
        //
        //! Exception with strerror(err)
        //
        //______________________________________________________________________
        class Exception : public Yttrium::Exception
        {
        public:
            static const size_t TitleSize = ClassSize - sizeof(int);
            
            explicit Exception(const int err, const char *fmt,...) noexcept Y_PRINTF_CHECK(3,4);
            virtual ~Exception() noexcept;
            Exception(const Exception &) noexcept;

            virtual const char *what() const noexcept; //!< title
            const int code;

        private:
            Y_DISABLE_ASSIGN(Exception);
            char title[TitleSize];
        };
    }
}

#endif

