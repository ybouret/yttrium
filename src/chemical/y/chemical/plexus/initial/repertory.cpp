
#include "y/chemical/plexus/initial/repertory.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Initial
        {
            Y_PROXY_IMPL(Repertory,my)

            const char * const Repertory:: CallSign = "Initial::Repertory";

            Repertory:: Repertory()
            {
            }

            Repertory:: ~Repertory() noexcept
            {
            }

            void  Repertory:: operator()(const XCode &xcode)
            {
                const Axioms axioms(xcode);
                if(!my.insert(axioms))
                    throw Specific::Exception(CallSign, "multiple Axioms '%s'", axioms.key().c_str());
            }

            const Axioms & Repertory:: query(const String &id) const
            {
                const Axioms * const p = my.search(id);
                if(!p) throw Specific::Exception(CallSign, "no Axioms '%s'", id.c_str());
                return *p;
            }

            const Axioms & Repertory:: query(const char * const id) const
            {
                const String _(id);
                return query(_);
            }

        }
        
    }

}
