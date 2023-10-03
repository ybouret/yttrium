


//! \file

#ifndef Y_Jive_Lexical_BString_Included
#define Y_Jive_Lexical_BString_Included 1

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
            //! Bracketed String
            //
            //
            //__________________________________________________________________
            class BString : public String_
            {
            public:

                //! setup
                template <typename ID> inline
                explicit BString(Lexer &    lx,
                                 const ID & id) :
                String_(lx,id,BRACKETS)
                {

                }

                //! cleanup
                virtual ~BString() noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(BString);
            };

        }

    }

}

#endif

