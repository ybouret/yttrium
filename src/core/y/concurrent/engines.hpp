
//! \file

#ifndef Y_Concurrent_Engines_Included
#define Y_Concurrent_Engines_Included 1

#include "y/concurrent/engine/interface.hpp"
#include "y/container/cxx/array.hpp"
#include "y/memory/allocator/dyadic.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        namespace Nucleus
        {
            class Engines
            {
            public:
                typedef Memory::Dyadic Model;

                virtual ~Engines() noexcept;
                explicit Engines() noexcept;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Engines);
            };
        }

    }

}

#endif

