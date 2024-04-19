
#include "y/chemical/reactive/components.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        const char Components:: LeftRightArrow[] = "<=>";

        Components:: ~Components() noexcept
        {

        }

        Components:: Components() :
        Proxy<const Component::Set>(),
        reac(),
        prod(),
        rstr(),
        pstr(),
        cdb()
        {
        }

        Components:: Components(const Components &other) :
        Proxy<const Component::Set>(),
        reac(other.reac),
        prod(other.prod),
        rstr(other.rstr),
        pstr(other.pstr),
        cdb(other.cdb)
        {
        }


        Components::ConstInterface & Components:: surrogate() const noexcept
        {
            return cdb;
        }

        void Components:: operator()(const int      nu,
                                     const Species &sp)
        {
            //------------------------------------------------------------------
            // look for species
            //------------------------------------------------------------------
            assert(0!=nu);
            static const char here[] = "Chemical::Components";
            const String &    name = sp.name;
            {
                const Component component(nu,sp);
                if(!cdb.insert(component)) throw Specific::Exception(here,"multiple species '%s'", name.c_str());
            }

            //------------------------------------------------------------------
            // insert corresponding actors
            //------------------------------------------------------------------
            Actors  *ac = 0;
            String  *ds = 0;
            {
                unsigned cf = 0;
                if(nu<0)
                {
                    // select reactants
                    ac = & Coerce(reac);
                    ds = & Coerce(rstr);
                    cf = unsigned( -int(nu) );
                }
                else
                {
                    // select products
                    ac = & Coerce(prod);
                    ds = & Coerce(pstr);
                    cf = unsigned( nu );
                }

                assert(0!=ac);
                try
                {
                    // grow actors
                    ac->pushTail( new Actor(cf,sp) );
                }
                catch(...)
                {
                    // emergency exit
                    (void) cdb.remove(name);
                    throw;
                }
            }

            try {
                assert(0!=ds);
                String s = ac->toString();
                ds->swapWith(s);
            }
            catch(...)
            {
                // emergency exit
                delete ac->popTail();
                (void) cdb.remove(name);
                throw;
            }

        }

        

        int Components:: charge() const noexcept
        {
            return prod.charge() - reac.charge();
        }

        bool Components:: contains(const Species &sp) const noexcept
        {
            const Component * const pc = cdb.search(sp.name);
            if(!pc) 
            {
                return false;
            }
            else
            {
                assert( &sp == &(pc->sp) );
                return true;
            }
        }

        bool Components:: sharesSpeciesWith(const Components &other) const noexcept
        {
            for(Component::Set::ConstIterator it=other.cdb.begin();it!=other.cdb.end();++it)
            {
                const Component &rhs = *it;
                if(contains(rhs.sp)) return true;
            }
            return false;
        }


    }

}


#include "y/type/utils.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Components:: Formatting::  Formatting() noexcept : rmax(0), pmax(0) {}
        Components:: Formatting:: ~Formatting() noexcept {}

        void   Components:: Formatting::   modernizeWith(const Components &components) noexcept
        {
            Coerce(pmax) = Max(pmax,components.pstr.size());
            Coerce(rmax) = Max(rmax,components.rstr.size());
        }

        std::ostream & Components::Formatting::print(std::ostream &os, const Components &components) const
        {
            for(size_t i=components.rstr.size();i<rmax;++i) os << ' ';
            os << components.rstr;

            os << LeftRightArrow;
            
            os << components.pstr;
            for(size_t i=components.pstr.size();i<pmax;++i) os << ' ';

            return os;
        }

    }

}


