//! \file

#ifndef Y_Jive_Syntax_Rule_Included
#define Y_Jive_Syntax_Rule_Included 1

#include "y/jive/syntax/xnode.hpp"
#include "y/graphviz/vizible.hpp"

namespace Yttrium
{
    class AddressBook;

    namespace Jive
    {

        namespace Syntax
        {

            //__________________________________________________________________
            //
            //
            //! helper for Rule API
            //
            //__________________________________________________________________
#define Y_JIVE_SYNTAX_RULE_API Lexer &lexer, Source &source, XTree &tree

            //__________________________________________________________________
            //
            //
            //
            //! Syntax Rule
            //
            //
            //__________________________________________________________________
            class Rule : public Entity, public Vizible
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef ArkPtr<String,Rule> Pointer; //!< alias


                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
            protected:
                //! create with name and uuid
                template <typename ID> inline
                explicit Rule(const ID &id, const uint32_t dw) :
                Entity(id), uuid(dw), self(0)  {}

            public:
                virtual ~Rule() noexcept;

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! cast to derived Rule
                template <typename T> inline
                const T *as() const noexcept {
                    assert(0!=self); assert(T::UUID==uuid);
                    return static_cast<const T*>(self);
                }

                //! interface
                virtual bool accepts(Y_JIVE_SYNTAX_RULE_API) const = 0;

                //! frail is accepts emtpy expression
                virtual bool isFrail() const noexcept = 0;

                //! !frail
                bool         isTough() const noexcept;

                //! endorse in book
                virtual void endorse(AddressBook &book)      const = 0;

                //! core GraphViz code: type, properties...
                virtual void vizCore(OutputStream &)       const = 0;

                //! post GraphViz code: linkage...
                virtual void vizLink(OutputStream &)       const = 0;

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const uint32_t uuid; //!< UUID
            protected:
                void  * const  self; //!< to access derived tule


                //! helper to set internal type
                template <typename T> static inline
                void I_am(T *rule) noexcept {
                    assert(0==rule->self);
                    Coerce(rule->self) = rule;
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Rule);
            };

            
        }

    }

}

#endif
