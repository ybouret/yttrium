
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
        //______________________________________________________________________
        //
        //
        //
        //! WEASEL is Equilbria And Species Elementary Language
        //
        //
        //______________________________________________________________________
        class Weasel : public Singleton<Weasel>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const      CallSign; //!< "Weasel"
            static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 12; //!< alias
            class Code;  // forward declaration

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! compile a module into species and equilibria
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
