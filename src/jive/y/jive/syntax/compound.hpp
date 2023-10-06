
//! \file

#ifndef Y_Jive_Syntax_Compound_Included
#define Y_Jive_Syntax_Compound_Included 1

#include "y/jive/syntax/internal.hpp"
#include "y/data/small/light/list/bare.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {

            //__________________________________________________________________
            //
            //
            //! base class for compound rules
            //
            //__________________________________________________________________
            typedef Small::BareLightList<const Rule> Manifest;

            //__________________________________________________________________
            //
            //
            //
            //! Base class for Compound Rule
            //
            //
            //__________________________________________________________________
            class Compound : public Internal, public Manifest
            {
            public:
                virtual ~Compound() noexcept; //!< cleanup

                virtual void endorse(AddressBook &book) const; //!< endorse manifest
                virtual void vizLink(OutputStream &)    const; //!< this to manifest

            protected:
                //! forward name and uuid
                template <typename ID>
                explicit Compound(const ID &id, const uint32_t dw) : 
                Internal(id,dw),
                Manifest()
                {
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Compound);
            };

        }

    }

}

#endif
