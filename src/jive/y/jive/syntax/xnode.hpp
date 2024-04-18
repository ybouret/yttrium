//! \file

#ifndef Y_Jive_Syntax_Node_Included
#define Y_Jive_Syntax_Node_Included 1

#include "y/jive/lexer.hpp"
#include "y/calculus/align.hpp"
#include "y/ptr/auto.hpp"
#include "y/graphviz/vizible.hpp"

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
            class XNode : public Object, public GraphViz:: Vizible
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
                void             fusion(Tree &tree)                 noexcept; //!< fusion tree content within chld
                const List   &   branch()                     const noexcept; //!< if internal
                const Lexeme &   lexeme()                     const noexcept; //!< if terminal
                size_t           weight()                     const noexcept; //!< number of terminals inside
                void             remove(const Tag &  tag)           noexcept; //!< remove intenal node matching tag
                
                //! bool REMOVE(const XNode &) noexcept
                template <typename REMOVE> inline
                void removeChildIf(REMOVE &bad) noexcept
                {
                    assert(IsInternal==type);
                    List  target;
                    List &source = chld;
                    while(source.size>0)
                    {
                        AutoPtr<XNode> node = source.popHead();
                        if(bad(*node)) continue;
                        target.pushTail(node.yield());
                    }
                    source.swapWith(target);
                }

                //______________________________________________________________
                //
                //
                // GraphViz
                //
                //______________________________________________________________
                void viz(OutputStream &)      const; //!< output code
                void graphViz(OutputStream &) const; //!< digraph { viz };

                //______________________________________________________________
                //
                //
                //! AST transformation
                //
                //______________________________________________________________
                static XNode *AST(XNode *root) noexcept;


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

                void          vizTerminal(OutputStream &) const; //!< viz for Terminal
                void          vizInternal(OutputStream &) const; //!< viz for Internal
                static XNode *astTerminal(XNode *)     noexcept;
                static XNode *astInternal(XNode *)     noexcept;

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


