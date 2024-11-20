#include "y/chemical/library.hpp"
#include "y/system/exception.hpp"
#include <iomanip>

namespace Yttrium
{
    namespace Chemical
    {
        Library:: ~Library() noexcept
        {
        }

        Library:: Library() :
        Assembly(),
        Proxy<const SpeciesSet>(),
        species()
        {

        }

        Library:: ConstInterface & Library:: surrogate() const noexcept
        {
            return species;
        }



        std::ostream & operator<<(std::ostream &os, const Library &lib)
        {
            const SpeciesSet &species = lib.species;
            if(species.size()<=0)
            {
                return os << "{}";
            }
            else
            {
                os << '{' << std::endl;
                for(SpeciesSet::ConstIterator it=species.begin();it!=species.end();++it)
                {
                    const Species &sp = **it;
                    os << "\t" << Justify(sp.name,lib.maxNameSize);
                    os << " | z=" << std::setw(3) << sp.z;
                    os << " | indx=";
                    Core::Display(os, sp.indx, Species::LEVELS);
                    os << std::endl;
                }
                os << '}';
                return os;
            }
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

            enroll(*sh);
            return *sh;
        }

    }

}

