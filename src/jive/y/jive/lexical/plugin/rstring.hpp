

//! \file

#ifndef Y_Jive_Lexical_RString_Included
#define Y_Jive_Lexical_RString_Included 1

#include "y/jive/lexical/plugin/string.hpp"

namespace Yttrium
{

    namespace Jive
    {

        namespace Lexical
        {

            //__________________________________________________________________
            //
            //
            //
            //! Raw string, single quote-separated
            //
            //
            //__________________________________________________________________
            class RString : public String_
            {
            public:

                //! setup
                template <typename ID> inline
                explicit RString(Lexer &    lx,
                                 const ID & id) :
                String_(lx,id,QUOTES)
                {

                }

                //! cleanup
                virtual ~RString() noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(RString);
            };

        }

    }

}

#endif

