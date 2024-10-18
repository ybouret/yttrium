
//! \file

#ifndef Y_Lingo_Logic_And_Included
#define Y_Lingo_Logic_And_Included 1

#include "y/lingo/pattern/logic.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        class And : public Logic
        {
        public:
            static const uint32_t UUID = Y_FOURCC('[','&', '&', ']');

            explicit And() noexcept;
            virtual ~And() noexcept;
            And(const And &);

            
        private:
            Y_DISABLE_ASSIGN(And);
        };
    }

}

#endif
