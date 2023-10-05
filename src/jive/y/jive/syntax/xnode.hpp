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

            //__________________________________________________________________
            //
            //
            //! Feature for XNode
            //
            //__________________________________________________________________
            enum Feature
            {
                IsTerminal, //!< a  terminal node
                IsInternal  //!< an internal node
            };

            //__________________________________________________________________
            //
            //
            //
            //! SyntaX-Tree node
            //
            //
            //__________________________________________________________________
            class XNode : public Object
            {
            public:
                //______________________________________________________________
                //
                //
                // Defintions
                //
                //______________________________________________________________
                typedef CxxListOf<XNode> List; //!< alias
                typedef AutoPtr<XNode>   Tree; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                virtual ~XNode() noexcept;                             //!< cleanup
                static   XNode * Create(const Internal &);             //!< create internal
                static   XNode * Create(const Terminal &, Lexeme *);   //!< create terminal

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                static   void    Expand(Tree &  tree,  XNode *node) noexcept; //!< expand tree
                static   void    Expand(Tree &  tree,  Tree  &sub ) noexcept; //!< expand tree
                static   void    BackTo(Lexer & lexer, XNode *node) noexcept; //!< node back to lexer
                static   void    BackTo(Lexer & lexer, Tree  &tree) noexcept; //!< return content of tree

                const List   & branch() const noexcept; //!< if internal
                const Lexeme & lexeme();                //!< if terminal

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const Rule   &rule; //!< creating rule
                const Feature type; //!< feature
                XNode        *next; //!< for list
                XNode        *prev; //!< for list
                XNode        *sire; //!< for tree

            private:
                Y_DISABLE_COPY_AND_ASSIGN(XNode);
                explicit XNode(const Rule &)           noexcept;
                explicit XNode(const Rule &, Lexeme *) noexcept;
                union
                {
                    Lexeme *lptr;                    //!< lexeme (auto) pointer
                    void   *wksp[Y_WORDS_FOR(List)]; //!< workspace for list
                };
                List       &chld;                    //!< alias to child

            };

            //__________________________________________________________________
            //
            //
            // Global definitions
            //
            //__________________________________________________________________
            typedef XNode::List    XList; //!< alias
            typedef XNode::Tree    XTree; //!< alias

        }

    }

}

#endif


