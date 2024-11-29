
#include "y/chemical/library.hpp"
#include "y/system/exception.hpp"
#include <iomanip>
#include "y/type/utils.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Library:: ~Library() noexcept
        {
        }

        const char * const Library:: CallSign = "Chemical::Library";
        
        Library:: Library() : Proxy<const Ingredients>(), db()
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
            const Ingredients &db = lib.db;
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

        real_t Library:: RanP( Random::Bits &ran ) noexcept
        {
            static const real_t amplitude = PMAX - PMIN;
            return Clamp<real_t>(PMIN, PMIN + ran.to<real_t>() * amplitude, PMAX);
        }

        xReal Library:: RanC( Random::Bits &ran ) noexcept
        {
            static const real_t ten = 10;
            return pow(ten, RanP(ran) );
        }

        void Library:: Conc(XWritable &  C,
                            Random::Bits &ran,
                            const real_t probaZero,
                            const real_t probaNegative) noexcept
        {
            for(size_t i=C.size();i>0;--i)
            {
                xReal &c = C[i];
                if( ran.to<real_t>() <= probaZero )
                {
                    c = 0;
                    continue;
                }

                c = RanC( ran );
                if( ran.to<real_t>() <= probaNegative )
                {
                    c.neg();
                }
            }
        }
    }

}

namespace Yttrium
{
    namespace Chemical
    {
        Ingredients:: Ingredients() : Species::Set(), Assembly()
        {
        }

        Ingredients:: ~Ingredients() noexcept
        {
            
        }

        void Ingredients:: mustInsert(const Species::Handle &handle) {
            if( !insert(handle) )
                throw Specific::Exception(Library::CallSign,"multiple species '%s'", handle->key().c_str() );
            enroll( *handle );
        }
    }

}
