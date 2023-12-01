//! \file

#ifndef Y_Concurrent_Split_Trek_Included
#define Y_Concurrent_Split_Trek_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    namespace Concurrent
    {
        class Trek
        {
        public:
            enum Kind
            {
                Increase,
                Decrease,
                Standard
            };
            
            virtual ~Trek() noexcept;
            explicit Trek(const Kind  ) noexcept;
            explicit Trek(const Trek &) noexcept;

            const Kind kind;

        private:
            Y_DISABLE_ASSIGN(Trek);
        };

    }

}

#endif


