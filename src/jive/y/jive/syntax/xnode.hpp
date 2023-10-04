//! \file

#ifndef Y_Jive_Syntax_Node_Included
#define Y_Jive_Syntax_Node_Included 1

#include "y/jive/lexer.hpp"
#include "y/calculus/align.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{
    namespace Jive
    {
        class Lexer;
        
        namespace Syntax
        {
            class Terminal;
            class Internal;
            class Rule;


            enum Feature
            {
                IsTerminal,
                IsInternal
            };


            class XNode : public Object
            {
            public:
                typedef CxxListOf<XNode> List;
                typedef AutoPtr<XNode>   Tree;

                virtual ~XNode() noexcept;
                static   XNode * Create(const Internal &);
                static   XNode * Create(const Terminal &,Lexeme *);
                static   void    Expand(Tree &  tree,  XNode *node) noexcept;
                static   void    BackTo(Lexer & lexer, XNode *node) noexcept;

                const List   & branch() const noexcept;
                const Lexeme & lexeme();

                const Rule   &rule;
                const Feature type;
                XNode        *next;
                XNode        *prev;
                XNode        *sire;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(XNode);
                explicit XNode(const Rule &)           noexcept;
                explicit XNode(const Rule &, Lexeme *) noexcept;
                union
                {
                    Lexeme *lptr;
                    void   *wksp[Y_WORDS_FOR(List)];
                };
                List       &chld;


            };

            typedef XNode::List    XList;
            typedef XNode::Tree    XTree;

        }

    }

}

#endif


