
//! \file

#ifndef Y_Chemical_Weasel_Included
#define Y_Chemical_Weasel_Included 1

#include "y/chemical/species/library.hpp"
#include "y/chemical/reactive/equilibria/lua.hpp"

#include "y/singleton.hpp"
#include "y/jive/module.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Rapid Obnoxious Species And Reactions Yoga
        //
        //
        //______________________________________________________________________
        class Weasel : public Singleton<Weasel>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            static const char * const      CallSign;       //!< ChemicalWeasel
            static const AtExit::Longevity LifeTime = 0;   //!< LifeTime
            class Compiler;                                // internal compiler

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! parse module, populate library and eqs
            void operator()(Jive::Module  *m,
                            Library       &lib,
                            LuaEquilibria &eqs);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Weasel);
            friend class Singleton<Weasel>;

            explicit Weasel();
            virtual ~Weasel() noexcept;
        };


    }

}

#endif
