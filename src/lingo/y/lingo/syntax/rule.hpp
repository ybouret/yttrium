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
#define Y_Lingo_Syntax_Args Lexer &lexer, Source &source, XNode * &tree

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
            protected:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

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
                virtual void         viz(OutputStream &fp)        const = 0; //!< GraphViz code
                virtual const char * vizShape()          const noexcept = 0; //!< shape
                virtual bool         accepts(Y_Lingo_Syntax_Args) const = 0; //!< accepts lexer/source
                virtual bool         robust()                     const = 0; //!< accepted is never empty
                bool                 flimsy()                     const;     //!< !robust()

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
                void        emitShape(OutputStream &)  const; //!< ",shape=" << vizShape()

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
            typedef CxxListOf<Rule> Rules;
        }
    }
}

#endif
