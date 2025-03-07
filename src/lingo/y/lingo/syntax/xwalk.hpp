
//! \file

#ifndef Y_Lingo_Syntax_XWalk_Included
#define Y_Lingo_Syntax_XWalk_Included 1

#include "y/lingo/syntax/xlist.hpp"


namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
            
            struct XWalk
            {

                //! void proc(node,depth)
                template <typename PROC> static inline
                void On(XNode * const root,
                        PROC &        proc,
                        const size_t  deep)
                {
                    proc(root,deep);
                    switch(root->type)
                    {
                        case XNode::Terminal:
                            break;

                        case XNode::Internal:
                            for(XNode *node=root->branch().head;node;node=node->next)
                                On(node,proc,deep+1);
                            break;
                    }
                }

            };


        }
    }

}

#endif

