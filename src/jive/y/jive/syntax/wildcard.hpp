//! \file

#ifndef Y_Jive_Syntax_Wildcard_Included
#define Y_Jive_Syntax_Wildcard_Included 1

#include "y/jive/syntax/internal.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {
            //__________________________________________________________________
            //
            //
            //
            //! Wildcard for another rule
            //
            //
            //__________________________________________________________________
            class Wildcard : public Internal
            {
            public:
                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! endorse this and rule
                virtual void endorse(AddressBook &book) const;

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                //! cleanup
                virtual ~Wildcard() noexcept;
            protected:

                //! setup with name, uuid and rule
                template <typename ID> inline
                explicit Wildcard(const ID      & id,
                                  const uint32_t  dw,
                                  const Rule    & rr) noexcept :
                Internal(id,dw), rule(rr) {}
                
                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                //! alias rule
                const Rule &rule;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Wildcard);
            };
        }

    }

}

#endif
