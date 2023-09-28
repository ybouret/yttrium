
//! \file

#ifndef Y_Jive_Lexical_Plugin_Included
#define Y_Jive_Lexical_Plugin_Included 1

#include "y/jive/lexical/scanner.hpp"

namespace Yttrium
{
    class XMLog;

    namespace Jive
    {

        namespace Lexical
        {
            class Plugin : public Scanner
            {
            public:
                virtual ~Plugin() noexcept;

                template <typename ID> inline
                explicit Plugin(const ID      &id,
                                const Scanner &parent);


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Plugin);
            };
        }

    }

}

#endif

