//! \file

#ifndef Y_Ptr_Included
#define Y_Ptr_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    namespace Core
    {
        class Ptr
        {
        public:
            static const char Nil[];

        protected:
            explicit Ptr() noexcept;
        public:
            virtual ~Ptr() noexcept;
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Ptr);
        };
    }

}

#endif

