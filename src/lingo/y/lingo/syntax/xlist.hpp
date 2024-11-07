
//! \file

#ifndef Y_Lingo_Syntax_XList_Included
#define Y_Lingo_Syntax_XList_Included 1

#include "y/lingo/syntax/xnode.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
            //__________________________________________________________________
            //
            //
            //
            //! List of XNodes
            //
            //
            //__________________________________________________________________
            class XList : public XNode::List
            {
            public:
                explicit XList() noexcept;               //!< setup
                virtual ~XList() noexcept;               //!< cleanup
                void     backToLexer(Lexer &)  noexcept; //!< send content back to lexer
                void     append(XNode * const) noexcept; //!< pushTail and link

            private:
                Y_DISABLE_COPY_AND_ASSIGN(XList);
            };
        }

    }

}

#endif
