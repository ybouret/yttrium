
#include "y/chemical/plexus/repertory.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Y_PROXY_IMPL(Repertory,my)

        const char * const Repertory:: CallSign = "Repertory";

        Repertory:: Repertory() : Proxy<const Initial::Axioms::Set>(), my()
        {
        }

        Repertory:: ~Repertory() noexcept
        {
        }

        std::ostream & operator<<(std::ostream &os, const Repertory &rep)
        {
            os << '{';
            if(rep->size()>0)
            {
                os << std::endl;
                for(Repertory::ConstIterator it=rep->begin();it!=rep->end();++it)
                {
                    os << '\t' << *it << std::endl;
                }
            }
            os << '}';
            return os;
        }

        void  Repertory:: operator()(const XCode &xcode)
        {
            const Initial::Axioms axioms(xcode);
            if(!my.insert(axioms))
                throw Specific::Exception(CallSign, "multiple Axioms '%s'", axioms.key().c_str());
        }

        const Initial::Axioms & Repertory:: query(const String &id) const
        {
            const Initial::Axioms * const p = my.search(id);
            if(!p) throw Specific::Exception(CallSign, "no Axioms '%s'", id.c_str());
            return *p;
        }

        const Initial::Axioms & Repertory:: query(const char * const id) const
        {
            const String _(id);
            return query(_);
        }


    }

}
