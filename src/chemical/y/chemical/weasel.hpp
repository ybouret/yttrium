//! \file


#ifndef Y_Chemical_Weasel_Included
#define Y_Chemical_Weasel_Included 1

#include "y/chemical/formula.hpp"
#include "y/chemical/reactive/equilibrium.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Weasel : public Singleton<Weasel>
        {
        public:
            static const char * const      CallSign;;
            static const AtExit::Longevity LifeTime = 100;
            static const char * const      Coef; //!< "Coef"

            class Parser;
            
        private:
            explicit Weasel();
            virtual ~Weasel() noexcept;
            friend class Singleton<Weasel>;
        };

    }
}

#endif

