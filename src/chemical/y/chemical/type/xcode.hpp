//! \file


#ifndef Y_Chemical_XNode_Included
#define Y_Chemical_XNode_Included 1

#include "y/lingo/syntax/xnode.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        typedef Lingo::Syntax::XNode XNode;   //!< alias
        typedef Lingo::Syntax::XList XList;   //!< alias
        typedef Lingo::Lexeme        Lexeme;  //!< alias
        typedef ArcPtr<const XNode>  XCode;   //!< for internal representations
        typedef ArcPtr<const String> XName;   //!< for storing names
        
    }

}

#endif
