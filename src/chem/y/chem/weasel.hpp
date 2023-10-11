
//! \file


#ifndef Y_Chemical_WEASEL_Included
#define Y_Chemical_WEASEL_Included 1

#include "y/chem/species/library.hpp"
#include "y/chem/reactive/lua/equilibria.hpp"
#include "y/jive/module.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //! WEASEL is Equilbria And Species Elementary Language
        class Weasel : public Singleton<Weasel>
        {
        public:
            static const char * const      CallSign;
            static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 12;
            class Code;

            void operator()(Jive::Module *, Library &, LuaEquilibria &);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Weasel);
            friend class Singleton<Weasel>;

            explicit Weasel();
            virtual ~Weasel() noexcept;

        };

    }

}

#endif
