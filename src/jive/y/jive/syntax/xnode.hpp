//! \file

#ifndef Y_Jive_Syntax_Node_Included
#define Y_Jive_Syntax_Node_Included 1

#include "y/jive/lexical/unit.hpp"
#include "y/calculus/align.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {
            enum Feature
            {
                IsTerminal,
                IsInternal
            };

            class XNode : public Object
            {
            public:
                typedef CxxListOf<XNode> List;


                virtual ~XNode() noexcept;
                static   XNode * Create();
                static   XNode * Create(Lexeme *);

                const Feature type;
                XNode        *next;
                XNode        *prev;
                XNode        *sire;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(XNode);
                explicit XNode()         noexcept;
                explicit XNode(Lexeme *) noexcept;
                union
                {
                    Lexeme *lptr;
                    void   *wksp[Y_WORDS_FOR(List)];
                };
                List       &list_;


            };

            typedef XNode::List XList;
            

        }

    }

}

#endif


