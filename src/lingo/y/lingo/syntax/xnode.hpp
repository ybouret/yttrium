//! \file

#ifndef Y_Lingo_Syntax_XNode_Included
#define Y_Lingo_Syntax_XNode_Included 1

#include "y/lingo/lexer.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
            class Rule;

            class XList;

            class XNode : public Object, public GraphViz::Vizible
            {
            public:
                typedef CxxListOf<XNode> List;

                enum Type
                {
                    Terminal,
                    Internal
                };


                static XNode * CreateFrom(const Rule &, Lexeme * const);
                static XNode * CreateFrom(const Rule &);

                virtual ~XNode() noexcept;

                const Rule & rule; //!< creating rule
                const Type   type; //!< from rule/constructor
                XNode *      sire; //!< for tree
                XNode *      next; //!< for list
                XNode *      prev; //!< for list

                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(XNode);
                union
                {
                    Lexeme * unit;
                    XList  * list;
                    void   * wksp[ Y_WORDS_FOR(List) ];
                };

                //! make a terminal
                explicit XNode(const Rule &   _rule,
                               Lexeme * const _unit) noexcept;

                //! make an internal
                explicit XNode(const Rule &   _rule) noexcept;

                

            };

            class XList : public XNode::List
            {
            public:
                explicit XList() noexcept;
                virtual ~XList() noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(XList);
            };
        }
    }
}

#endif

