
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
        //
        //! Exception with strerror(err)
        //
        //
        //______________________________________________________________________
        class Exception : public Yttrium::Exception
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const size_t TitleSize = ClassSize; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup with fixed header and formated message
            explicit Exception(const char *header, const char *fmt,...) noexcept Y_PRINTF_CHECK(3,4);
            virtual ~Exception()         noexcept; //!< cleanup
            Exception(const Exception &) noexcept; //!< copy

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual const char *what() const noexcept; //!< return title

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
        //
        //! Exception with strerror(errno)
        //
        //
        //______________________________________________________________________
        class Exception : public Yttrium::Exception
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definition
            //
            //__________________________________________________________________
            static const size_t TitleSize = ClassSize - sizeof(int); //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup with errno and formated message
            explicit Exception(const int err, const char *fmt,...) noexcept Y_PRINTF_CHECK(3,4);
            virtual ~Exception()         noexcept; //!< cleanup
            Exception(const Exception &) noexcept; //!< copy

            //__________________________________________________________________
            //
            //
            // Method
            //
            //__________________________________________________________________
            virtual const char *what() const noexcept; //!< title


            //__________________________________________________________________
            //
            //
            // Member
            //
            //__________________________________________________________________
            const int code; //!< original errno

        private:
            Y_DISABLE_ASSIGN(Exception);
            char title[TitleSize];
        };
    }

#if defined(Y_WIN)
    namespace Win32
    {
        //______________________________________________________________________
        //
        //
        //
        //! Exception with FormatMessage
        //
        //
        //______________________________________________________________________
        class Exception : public Yttrium::Exception
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definition
            //
            //__________________________________________________________________
            static const size_t TitleSize = ClassSize - sizeof(uint32_t); //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup with errno and formated message
            explicit Exception(const uint32_t err, const char* fmt, ...) noexcept Y_PRINTF_CHECK(3, 4);
            virtual ~Exception()         noexcept; //!< cleanup
            Exception(const Exception&) noexcept; //!< copy

            //__________________________________________________________________
            //
            //
            // Method
            //
            //__________________________________________________________________
            virtual const char* what() const noexcept; //!< title


            //__________________________________________________________________
            //
            //
            // Member
            //
            //__________________________________________________________________
            const uint32_t code; //!< original errno

        private:
            Y_DISABLE_ASSIGN(Exception);
            char title[TitleSize];
        };
    }
#endif

}

#endif

