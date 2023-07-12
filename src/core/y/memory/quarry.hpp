
//! \file

#ifndef Y_Memory_Pages_Included
#define Y_Memory_Pages_Included 1

#include "y/data/list.hpp"

namespace Yttrium
{

    namespace Memory
    {


        class Quarry
        {
        public:
            class Stone
            {
            public:
                Stone() noexcept;
                ~Stone() noexcept;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Stone);
            };

            class Vein
            {

            };

            virtual ~Quarry() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Quarry);
        };
    }

}

#endif

