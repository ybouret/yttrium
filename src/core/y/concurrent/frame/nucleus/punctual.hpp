//! \file

#ifndef Y_CONCURRENT_FRAME_PUNCTUAL_INCLUDED
#define Y_CONCURRENT_FRAME_PUNCTUAL_INCLUDED 1

#include "y/config/starting.hpp"
#include <iosfwd>

namespace Yttrium
{
    namespace Concurrent
    {
        namespace Nucleus
        {
            class  Punctual
            {
            public:
                explicit Punctual() noexcept;
                virtual ~Punctual() noexcept;

                friend std::ostream & operator<<(std::ostream &, const Punctual &);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Punctual);
            };
        }
    }
}

#endif

