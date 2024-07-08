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
            class Compiler : public Singleton<Compiler>
            {
            public:
                static const char * const      CallSign;
                static const AtExit::Longevity LifeTime = 89;
                class Code;

                void operator()(Library       &usrLib,
                                LuaEquilibria &usrEqs,
                                Jive::Module  *input,
                                const bool     saveAst = false);

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
