//! \file


#ifndef Y_Chemical_Weasel_Included
#define Y_Chemical_Weasel_Included 1

#include "y/chemical/type/xnode.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Weasel : public Singleton<Weasel>
        {
        public:
            static const char * const      CallSign;;
            static const AtExit::Longevity LifeTime = 100;

            class Parser;
            
        private:
            explicit Weasel();
            virtual ~Weasel() noexcept;
            friend class Singleton<Weasel>;
        };

    }
}

#endif

