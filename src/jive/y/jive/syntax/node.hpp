//! \file

#ifndef Y_Jive_Syntax_Node_Included
#define Y_Jive_Syntax_Node_Included 1

#include "y/jive/lexical/unit.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {

            class Node : public Object
            {
            public:


                Node *next;
                Node *prev;
                Node *parent;
            private:
                Y_DISABLE_ASSIGN(Node);
            };

        }

    }

}

#endif


