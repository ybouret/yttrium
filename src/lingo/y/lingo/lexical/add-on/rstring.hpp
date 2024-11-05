
//! \file

#ifndef Y_Lingo_Lexical_RString_Included
#define Y_Lingo_Lexical_RString_Included 1

#include "y/lingo/lexical/add-on/string.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        namespace Lexical
        {
            //__________________________________________________________________
            //
            //
            //
            //! Single Quoted String
            //
            //
            //__________________________________________________________________
            class RString : public String_
            {
            public:
                //! setup
                template <typename LABEL> inline
                explicit RString(Lexer       & rootLexer,
                                 const LABEL & addOnUUID) :
                String_(rootLexer,addOnUUID,'\'','\'')
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

