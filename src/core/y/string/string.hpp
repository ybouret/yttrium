//! \file

#ifndef Y_String_Core_Included
#define Y_String_Core_Included 1

#include "y/counted.hpp"
#include "y/object.hpp"

namespace Yttrium
{

    namespace Core
    {
        template <typename T>
        class String : public Object, public Counted
        {
        public:
            explicit String();
            virtual ~String() noexcept;

        private:

        };
    }
}

#endif

