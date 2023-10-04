
//! \file

#ifndef Y_Jive_Syntax_Rule_Included
#define Y_Jive_Syntax_Rule_Included 1

#include "y/jive/syntax/xnode.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {

#define Y_JIVE_SYNTAX_RULE_API Lexer &lexer, Source &source, XTree &tree
            class Rule : public Entity
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
            protected:
                template <typename ID> inline
                explicit Rule(const ID &id, const uint32_t dw) :
                Entity(id), uuid(dw), self(0) {}

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

                virtual bool accepts(Y_JIVE_SYNTAX_RULE_API) const = 0;

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
                template <typename T> inline
                void I_am() noexcept { assert(0==self); T *here = this; Coerce(self) = here; }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Rule);
            };
        }

    }

}

#endif
