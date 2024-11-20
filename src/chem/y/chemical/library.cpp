#include "y/chemical/library.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Library:: ~Library() noexcept
        {
        }

        Library:: Library() :
        Assembly(),
        Proxy<const SpeciesSet>()
        {

        }

        Library:: ConstInterface & Library:: surrogate() const noexcept
        {
            return species;
        }
        
        std::ostream & operator<<(std::ostream &os, const Library &lib)
        {
            return os << lib.species;
        }

        const Species & Library:: operator()(const String &sid, const int z)
        {
            {
                const Species::Handle *pps = species.search(sid);
                if(0!=pps) {
                    const Species &sp = **pps;
                    if(z!=sp.z) throw Specific::Exception(sid.c_str(), "mismatch charge %d instead of %d", z, sp.z);
                    return sp;
                }

                assert(0==pps);
            }

            const Species::Handle sh( new Species(sid,z,species.size()+1) );
            if(!species.insert(sh))
                throw Specific::Exception(sid.c_str(),"unexpected failure to insert into Library");
            return *sh;
        }

    }

}

