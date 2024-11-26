
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

        const char * const Library:: CallSign = "Chemical::Library";
        
        Library:: Library() : Proxy<const SpeciesDB>(), db()
        {

        }

        Library::ConstInterface & Library:: surrogate() const noexcept
        {
            return db;
        }

        const Species & Library:: get(const Formula &formula)
        {
            {
                const Species::Handle * const pps = db.search( formula.name );
                if(0!=pps)
                    return **pps;;
            }

            const Species::Handle handle( new Species(formula,db.size()+1) );
            db.mustInsert(handle);
            return *handle;
        }

        std::ostream & operator<<(std::ostream &os, const Library &lib)
        {
            const SpeciesDB &db = lib.db;
            if(db.size()<=0)
                return os << "{}";
            os << '{' << std::endl;
            for(Library::ConstIterator it=lib->begin();it!=lib->end();++it)
            {
                const Species &sp = **it;
                lib->print(os, "\t", sp, "", Justify::Left);
                os << " | z=" << std::setw(3) << sp.z;
                os << " | indx=";
                Core::Display(os,sp.indx,Indexed::Levels);
                os <<  std::endl;
            }
            os << '}';
            return os;
        }

    }

}

namespace Yttrium
{
    namespace Chemical
    {
        SpeciesDB:: SpeciesDB() : Species::Set(), Assembly()
        {
        }

        SpeciesDB:: ~SpeciesDB() noexcept
        {
            
        }

        void SpeciesDB:: mustInsert(const Species::Handle &handle) {
            if( !insert(handle) )
                throw Specific::Exception(Library::CallSign,"multiple species '%s'", handle->key().c_str() );
            enroll( *handle );
        }
    }

}
