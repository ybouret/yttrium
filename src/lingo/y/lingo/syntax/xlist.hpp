
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
            class XList : public XNode::List
            {
            public:
                explicit XList() noexcept;
                virtual ~XList() noexcept;
                void     backToLexer(Lexer &) noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(XList);
            };
        }

    }

}

#endif
