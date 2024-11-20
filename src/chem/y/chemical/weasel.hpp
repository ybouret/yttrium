
//! \file

#ifndef Y_Chemical_Weasel_Included
#define Y_Chemical_Weasel_Included 1

#include "y/chemical/species.hpp"
#include "y/lingo/caption.hpp"
#include "y/singleton.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Weasel : public Singleton<Weasel>
        {
        public:
            static const char * const      CallSign;
            static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 30;
            class Parser;

            const Lingo::Caption caption;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Weasel);
            friend class Singleton<Weasel>;
            virtual ~Weasel() noexcept;
            explicit Weasel();
        };
    }

}

#endif

