
//! \file


#ifndef Y_Chemical_Fragment_Included
#define Y_Chemical_Fragment_Included 1

#include "y/chemical/type/assembly.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Fragment : public Assembly
        {
        public:
            explicit Fragment();
            virtual ~Fragment() noexcept;

            Assembly reac;
            Assembly prod;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Fragment);
        };

    }

}


#endif

