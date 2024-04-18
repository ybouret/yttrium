
#include "y/chemical/reactive/components.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Components:: ~Components() noexcept
        {

        }

        Components:: Components() :
        reac(),
        prod(),
        cdb()
        {
        }

        Components:: Components(const Components &other) :
        reac(other.reac),
        prod(other.prod),
        cdb(other.cdb)
        {
        }
        
        void Components:: operator()(const int nu, const Species &sp)
        {
            assert(0!=nu);

            static const char here[] = "Chemical::Components";
            const String &    name = sp.name;
            {
                const Component component(nu,sp);
                if(!cdb.insert(component)) throw Specific::Exception(here,"multiple species '%s'", name.c_str());
            }

            Actors *ac = 0;
            {
                unsigned cf = 0;
                if(nu<0)
                {
                    ac = & Coerce(reac);
                    cf = unsigned( -int(nu) );
                }
                else
                {
                    ac = & Coerce(prod);
                }
            }


        }

    }

}


