#include "y/chemical/library.hpp"
#include "y/system/exception.hpp"
#include <iomanip>

namespace Yttrium
{
    namespace Chemical
    {

        const char * const Library::CallSign = "Chemical::Library";

        Library:: ~Library() noexcept
        {
        }

        Library:: Library() :
        Proxy<const SpeciesSet>(),
        Assembly(),
        Serializable(),
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
                for(Library::ConstIterator it=species.begin();it!=species.end();++it)
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

            return mustInsert( new Species(sid,z,species.size()+1) );

        }

        const Species & Library:: mustInsert(Species * const sp)
        {
            assert(0!=sp);
            const Species::Handle sh( sp );

            // check correct
            const size_t expected = species.size()+1;
            const size_t topLevel = sh->indx[TopLevel];
            if(topLevel != expected )
                throw Specific::Exception(sp->name.c_str(), "invalid species top-level index=%u instead of %u", unsigned(topLevel), unsigned(expected) );

            // try to insert
            if(!species.insert(sh))
                throw Specific::Exception(sp->name.c_str(),"unexpected failure to insert into %s",CallSign);

            // update
            enroll(*sh);
            return *sh;
        }


    }

}

#include "y/chemical/type/io.hpp"
#include "y/stream/input.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        size_t Library:: serialize(OutputStream &fp) const
        {
            const size_t nsp = species.size();
            size_t       res = fp.emitVBR(nsp);
            
            for(ConstIterator it=species.begin();it!=species.end();++it) {
                const Species &sp = **it;
                res += sp.serialize(fp);
            }

            return res;
        }


        void Library:: load(InputStream &fp)
        {
            if(species.size()>0) throw Specific::Exception(CallSign,"must be empty to be loaded");
            VarInfo      info;
            const size_t nsp = fp.readVBR<size_t>( info("%s.species",CallSign) );
            for(size_t i=1;i<=nsp;++i)
            {
                (void) mustInsert( Species::Read(fp,i) );
            }
        }
    }

}

