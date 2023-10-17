
#include "y/chem/species/library.hpp"
#include "y/system/exception.hpp"
#include <iomanip>

namespace Yttrium
{
    namespace Chemical
    {
        const char * const Library:: CallSign = "Chemical::Library";


        double Library:: Conc(Random::Bits &ran) noexcept
        {
            static const double pmin = Log10Min;
            static const double pmax = Log10Max;
            const double p = pmin + ran.to<double>()*(pmax-pmin);
            return pow(10.9,p);
        }


        Library:: ~Library() noexcept
        {
            
        }

        Library:: Library(const Library &lib) :
        Entities(lib),
        Proxy<const SpeciesDB>(),
        db(lib.db)
        {

        }

        Library:: Library() :
        Entities(),
        Proxy<const SpeciesDB>(),
        db()
        {
        }

        Library::ConstInterface & Library:: surrogate() const noexcept
        {
            return db;
        }

        const Species & Library:: query(const String &name, const int z)
        {
            const Species::Pointer *pps = db.search(name);
            if(!pps)
            {
                // create new species
                Species               *s = new Species(name,z);
                const Species::Pointer p(s);
                if(!db.insert(p))
                    throw Specific::Exception(CallSign, "Unexpected insert [%s] failure!", name.c_str());

                // updated entities
                updateWith(*s);

                // initialize indices
                const size_t I = (*this)->size();
                for(unsigned i=0;i<Levels;++i) Coerce(s->indx[i]) = I;

                return *s;
            }
            else
            {
                const Species &s = **pps;
                if(s.z!=z) throw Specific::Exception(CallSign, "[%s] charge mismatch!", name.c_str());
                return s;
            }
        }

        const Species & Library:: query(const char * name, const int z)
        {
            const String _(name);
            return query(_,z);
        }

        std::ostream & operator<<(std::ostream &os, const Library &lib)
        {
            os << '{' << std::endl;
            for(Library::ConstIterator it=lib->begin();it!=lib->end();++it)
            {
                const Species &sp = **it;
                lib.pad(os << "  " << sp,sp)
                << " | z=" << std::setw(3) << sp.z
                << " | i=" << std::setw(3) << sp.indx[TopLevel]
                << std::endl;
            }
            os << '}';
            return os;
        }
    }

}


