
#include "y/information/entropic/alphabet.hpp"
#include "y/text/ascii/printable.hpp"
#include "y/memory/out-of-reach.hpp"
#include <iomanip>

namespace Yttrium
{

    namespace Information
    {

        namespace Entropic
        {



            Alphabet:: Alphabet(const bool useEOS) noexcept :
            used(),
            unit(0),
            nyt(0),
            eos(0),
            sumf(0),
            ctrl(),
            nctl(0),
            wksp()
            {
                Coerce(unit) = static_cast<Unit *>( Y_STATIC_ZARR(wksp) );
                Coerce(nyt)  = unit + Unit::NYT;
                Coerce(eos)  = unit + Unit::EOS;
                memset(ctrl,0,sizeof(ctrl));

                for(uint16_t i=0;i<Unit::Encoding;++i)
                {
                    new (&unit[i]) Unit(i,8);
                }

                for(uint16_t i=Unit::Encoding;i<Unit::Universe;++i)
                {
                    new ( &unit[i] ) Unit(i,0);
                }

                {            ctrl[Coerce(nctl)++] = nyt; }
                if(useEOS) { ctrl[Coerce(nctl)++] = eos; }

                addCtrl();
            }

            Alphabet:: ~Alphabet() noexcept
            {
                used.reset();
                for(uint16_t i=0;i<Unit::Universe;++i)
                    Destruct( &unit[i] );

            }

            void Alphabet:: addCtrl() noexcept
            {
                assert(0==used.size);
                for(size_t i=0;i<nctl;++i)
                {
                    Unit *ctl = ctrl[i]; assert(0!=ctl);
                    used.pushTail(ctl);
                }
            }


            void Alphabet:: reset() noexcept
            {
                used.reset();
                for(uint16_t i=0;i<Unit::Universe;++i)
                    unit[i].reset();
                sumf = 0;
                addCtrl();
            }

            void Alphabet:: reduceFrequencies() noexcept
            {
                sumf = 0;
                for(Unit *u=used.head;u;u=u->next)
                {
                    u->reduceFrequency();
                    sumf += u->freq;
                }
            }

            void Alphabet:: display(std::ostream &os) const
            {
                os << "<Alphabet used='" << used.size << "'>" << std::endl;
                for(const Unit *u=used.head;u;u=u->next)
                {
                    u->display(os) << std::endl;
                }
                os << "<Alphabet/>" << std::endl;
            }


        }

    }

}
