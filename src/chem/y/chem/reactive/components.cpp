#include "y/chem/reactive/components.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Components:: ~Components() noexcept {}

        Components:: Components() :
        reac(),
        prod(),
        db()
        {
        }

        Components::ConstInterface & Components::surrogate() const noexcept { return db; }


        const char * const Components:: CallSign = "Chemical::Components";

        void Components:: operator()(const int nu, const Species &sp)
        {
            if(0==nu) throw Specific::Exception(CallSign,"zero [%s]", sp.name.c_str());

            const Component cc(sp,nu);
            if(!db.insert(cc)) throw Specific::Exception(CallSign,"multiple [%s]", sp.name.c_str());

            const Actors &actors = nu>0 ? prod : reac;
            try
            {
                Coerce(actors).pushTail( new Actor(sp,abs(nu)) );
            }
            catch(...)
            {
                db.remove(sp.name);
                throw;
            }
        }

        String Components:: toString() const
        {
            String res = reac.toString();
            res += " <=> ";
            res += prod.toString();
            return res;
        }

        bool Components:: linkedTo(const Components &other) const noexcept
        {
            if( &other == this )
            {
                return true;
            }
            else
            {
                const ConstIterator last = other.db.end();
                for(ConstIterator it=other.db.begin();it!=last;++it)
                {
                    const Component &cc = *it;
                    const Component *pc = db.search(cc.sp.name);
                    if(pc)
                    {
                        assert( & (pc->sp) == & (cc.sp) );
                        return true;
                    }
                }
                return false;
            }
        }

        
        void Components:: insertSpeciesIn(AddressBook &book) const
        {
            const ConstIterator last = db.end();
            for(ConstIterator it = db.begin();it!=last;++it)
            {
                const Species &sp = (*it).sp;
                book |= sp;
            }
        }

    }

}

