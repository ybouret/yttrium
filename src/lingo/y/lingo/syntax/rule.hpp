//! \file

#ifndef Y_Lingo_Syntax_Rule_Included
#define Y_Lingo_Syntax_Rule_Included 1

#include "y/lingo/syntax/xlist.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
            //__________________________________________________________________
            //
            //
            //! helper to define the 'self' in constructors
            //
            //__________________________________________________________________
#define Y_Lingo_Syntax_Rule(TYPE) Coerce(self) = static_cast<TYPE *>(this)

            //__________________________________________________________________
            //
            //
            //! arguments to accept
            //
            //__________________________________________________________________
#define Y_Lingo_Syntax_Args Lexer &lexer, Source &source, XNode * &tree, const size_t depth

            //__________________________________________________________________
            //
            //
            //! helper to trace lexical processing
            //
            //__________________________________________________________________
#define Y_Lingo_Syntax_Rule_Emit(DEPTH,MSG) do { if(Syntax::Rule::Trace) { Core::Indent(std::cerr,(DEPTH)) << MSG << std::endl; } } while(false)

            //__________________________________________________________________
            //
            //
            //
            //! Rule interface
            //
            //
            //__________________________________________________________________
            class Rule : public Entity, public GraphViz::Vizible
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef CxxListOf<Rule> List;  //!< alias
                static bool             Trace; //!< default to false

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
            protected:
                //! setup with name and uuid
                template <typename NAME> inline
                explicit Rule(const NAME &   _name,
                              const uint32_t _uuid):
                Entity(_name,AsCaption),
                GraphViz::Vizible(),
                uuid(_uuid),
                self(0), next(0), prev(0)
                {
                }

            public:
                //! cleanup
                virtual ~Rule() noexcept;

                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________
                virtual bool         accepts(Y_Lingo_Syntax_Args) const = 0; //!< accepts lexer/source
                virtual bool         robust()                     const = 0; //!< accepted is never empty
                bool                 flimsy()                     const;     //!< !robust()
                virtual void         vizMark(OutputStream &)      const = 0; //!< after label in node modified
                virtual void         vizLink(OutputStream &)      const;     //!< default : do nothing

                //______________________________________________________________
                //
                //
                // Methods for GraphViz
                //
                //______________________________________________________________
                void vizArrow(OutputStream &, const Rule &) const; //!< make a simple arroe

                //! make optionally numbered arrows
                template <typename LIST> inline
                void vizArrows(OutputStream &fp, const LIST &rules) const
                {
                    switch(rules.size) {
                        case 0: return;
                        case 1: vizArrow(fp, **(rules.head) ); return;
                        default:
                            break;
                    }

                    unsigned i = 1;
                    for(const typename LIST::NodeType *node = rules.head;node;node=node->next,++i)
                    {
                        Arrow(fp,this,&**node) << '[';
                        Endl( fp("label=\"%u\"",i) << ']' );
                    }
                }

                void vizCode(OutputStream &fp) const; //!< produce from vizMark and vizLink GraphViz code



                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                void        graphViz(OutputStream &fp) const; //!< standalone GrapViz, to debug
                bool        isInternal()      const noexcept; //!< uuid != Terminal::UUID
                bool        isTerminal()      const noexcept; //!< uuid == Terminal::UUID
                XNode::Type typeOfNode()      const noexcept; //!< depends on uuid

                //! conversion to derived class
                template <typename RULE> inline
                RULE * as() noexcept {
                    assert(RULE::UUID==uuid);
                    return static_cast<RULE *>(self);
                }


                //! conversion to derived class
                template <typename RULE> inline
                const RULE * as() const noexcept {
                    assert(RULE::UUID==uuid);
                    return static_cast<const RULE *>(self);
                }

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
            public:
                const uint32_t uuid; //!< identifier

            protected:
                void * const   self; //!< pointer to derived class
            public:
                Rule *         next; //!< for list
                Rule *         prev; //!< for list

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Rule);
            };


            //__________________________________________________________________
            //
            //
            //
            //! List of rules for grammar
            //
            //
            //__________________________________________________________________
            class Rules : public Rule::List
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Rules() noexcept; //!< setup
                virtual ~Rules() noexcept; //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                const Rule * query(const Caption &)    const noexcept; //!< query by name
                const Rule * query(const String  &)    const noexcept; //!< query by name
                const Rule * query(const char * const) const noexcept; //!< query by name


                //! query a terminal
                template <typename UID> inline
                const Terminal * queryTerminal(const UID &uid) const noexcept {
                    const Rule * rule = query(uid);
                    if(!rule || ! rule->isTerminal() ) return 0;
                    return rule->as<Terminal>();
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Rules);
            };
        }
    }
}

#endif
