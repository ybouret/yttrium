//! \file

#ifndef Y_Chemical_Weasel_Compiler_Included
#define Y_Chemical_Weasel_Compiler_Included 1

#include "y/chemical/library.hpp"
#include "y/chemical/reactive/equilibria/lua.hpp"
#include "y/jive/module.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Weasel
        {
            //__________________________________________________________________
            //
            //
            //
            //! Compiler as a singleton
            //
            //
            //__________________________________________________________________
            class Compiler : public Singleton<Compiler>
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                static const char * const      CallSign;        //!< "Weasel"
                static const AtExit::Longevity LifeTime = 89;   //!< life time
                class Code;

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! two stages compilation of input
                void operator()(Library       &usrLib,
                                LuaEquilibria &usrEqs,
                                Jive::Module  *input);

                void record(const String       &entry); //!< record precompiled
                void record(const char * const entry);  //!< record precompiled

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Compiler);
                friend class Singleton<Compiler>;

                explicit Compiler();
                virtual ~Compiler() noexcept;

            };
        }
    }
}

#endif
