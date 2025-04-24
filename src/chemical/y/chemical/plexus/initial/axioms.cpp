
#include "y/chemical/plexus/initial/axioms.hpp"
#include "y/chemical/plexus/initial/design.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Initial
        {

            static inline
            const String * extractLogoFrom(const XCode &code)
            {
                assert(code->defines<Design>());
                const XList        & list = code->branch(); assert(list.size>0);
                const XNode * const  logo = list.head;      assert(Design::Logo==logo->name());
                const String         uuid = logo->lexeme().toString();
                return new String(uuid);
            }

            Axioms:: Axioms(const XCode &code) :
            Entity( extractLogoFrom(code) ),
            xcode(code)
            {
            }


            Axioms:: ~Axioms() noexcept
            {
            }
            
        }

    }

}


