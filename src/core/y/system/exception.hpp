
//! \file

#ifndef Y_System_Exception_Included
#define Y_System_Exception_Included 1

#include "y/exception.hpp"

namespace Yttrium
{

    namespace Libc
    {

        //______________________________________________________________________
        //
        //
        //! Exception
        //
        //______________________________________________________________________
        class Exception : public Yttrium::Exception
        {
        public:
            static const size_t HereSize = BaseSize - sizeof(int);
            
            explicit Exception(const int err, const char *fmt,...) noexcept Y_PRINTF_CHECK(3,4);
            virtual ~Exception() noexcept;
            Exception(const Exception &) noexcept;

            virtual const char *what() const noexcept;

            const int code;

        private:
            Y_DISABLE_ASSIGN(Exception);
            char title[HereSize];
        };
    }
}

#endif

