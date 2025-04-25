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
            class Terminal;
            class Internal;

            typedef Quantized XNodeObject; //!< alias

            //__________________________________________________________________
            //
            //
            //
            //! Node holding result of Rule acceptance
            //
            //
            //__________________________________________________________________
            class XNode :
            public XNodeObject,
            public Counted,
            public Serializable,
            public GraphViz::Vizible
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef CxxListOf<XNode> List; //!< alias

                //! type of Node
                enum Type
                {
                    Terminal, //!< holds a  Lexeme
                    Internal  //!< holds an XList
                };

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! recursive duplicate
                explicit XNode(const XNode &node);

                //! create from Syntax::Terminal rule + lexeme
                static XNode * CreateFrom(const Syntax::Terminal &, Lexeme * const);

                //! create from Syntax::Terminal + synthetic data
                static XNode * CreateFrom(const Syntax::Terminal &, const String &);


                //! create from Syntax::Internal rule
                static XNode * CreateFrom(const Syntax::Internal &);

                //! duplicate
                static XNode * Duplicate(const XNode * const root);

                //! cleanup
                virtual ~XNode() noexcept;

                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________
                virtual size_t serialize(OutputStream &) const;      //!< [Serializable]


                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                void           viz(OutputStream &)       const; //!< write graphviz code
                void           graphViz(OutputStream &)  const; //!< write standalone graphvix code

                //! returning to lexer a rejected Xnode
                static void BackToLexer(Lexer &, XNode * const) noexcept;

                //! Grow tree with a newly created node
                static void Grow(XNode * &tree, XNode * const node) noexcept;

                //! full consistency check, mostly for debug
                bool isWellFormed() const;



                Lexeme &        lexeme()              noexcept; //!< access lexeme if Terminal
                const Lexeme &  lexeme()        const noexcept; //!< access lexeme if Terminal
                XList        &  branch()              noexcept; //!< access XList if Internal
                const XList  &  branch()        const noexcept; //!< access XList if Internal
                void            fusion(XNode * const) noexcept; //!< take ownership of a node
                void            fusion(XList &)       noexcept; //!< take ownership of a list
                const String  & name()          const noexcept; //!< rule name
                const XNode   & last()                 const noexcept; //!< last from this position
                const Lexeme  * lastLexeme()           const noexcept; //!< previous from last()
                void            appendTo(Exception &)  const noexcept; //!< if lexeme is Terminal
                static XNode  * AST(XNode * const)           noexcept; //!< recursive AST rules
                static bool     AreEqual(const XNode &,const XNode &); //!< recursive equality
                
                friend bool operator==(const XNode &lhs, const XNode &rhs); //!< test equality
                friend bool operator!=(const XNode &lhs, const XNode &rhs); //!< test difference
                bool is(const char * const) const noexcept;                 //!< check name
                bool is(const String &)     const noexcept;                 //!< check name

                //! return is(CLASS::CallSign)
                template <typename CLASS> inline
                bool defines() const noexcept
                {
                    return is( CLASS::CallSign );
                }



                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const Rule & rule; //!< creating rule
                const Type   type; //!< from rule/constructor
                XNode *      sire; //!< for tree
                XNode *      next; //!< for list
                XNode *      prev; //!< for list

            private:
                Y_DISABLE_ASSIGN(XNode);
                union
                {
                    Lexeme * unit;
                    void   * wksp[ Y_WORDS_FOR(List) ];
                };
                void    zero() noexcept;
                XList & list() const noexcept;


                //! make a terminal
                explicit XNode(const Syntax::Terminal &   _rule,
                               Lexeme * const             _unit) noexcept;

                //! make an internal
                explicit XNode(const Syntax::Internal &   _rule) noexcept;

                void vizTerminal(OutputStream &) const;
                void vizInternal(OutputStream &) const;
            };

        
        }
    }
}

#endif

