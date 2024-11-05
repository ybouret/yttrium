//! \file

#ifndef Y_Lingo_Lexical_JString_Included
#define Y_Lingo_Lexical_JString_Included 1

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
            //! Double Quoted String
            //
            //
            //__________________________________________________________________
            class JString : public String_
            {
            public:
                //! setup
                template <typename LABEL> inline
                explicit JString(Lexer &        rootLexer,
                                 const LABEL & addOnUUID) :
                String_(rootLexer,addOnUUID,'"','"')
                {
                }

                //! cleanup
                virtual ~JString() noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(JString);
            };
        }

    }

}

#endif

