//! \file

#ifndef Y_String_Core_Included
#define Y_String_Core_Included 1

#include "y/counted.hpp"
#include "y/object.hpp"
#include "y/container/writable.hpp"

namespace Yttrium
{

    namespace Core
    {
        template <typename T>
        class String : public Object, public Counted, public Writable<T>
        {
        public:
            Y_ARGS_EXPOSE(T);

            explicit String();
            virtual ~String() noexcept;

            virtual ConstType & operator[](const size_t index) const noexcept;
            virtual Type      & operator[](const size_t index)       noexcept;
            virtual size_t      size()     const noexcept;
            virtual size_t      capacity() const noexcept;
            
        private:
            class Impl;
            Impl *impl;
        };
    }
}

#endif

