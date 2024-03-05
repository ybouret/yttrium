
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
            emit( & Alphabet::Init ),
            unit(0),
            nyt(0),
            eos(0),
            sumf(0),
            nchr(0),
            ctrl(),
            nctl(0),
            wksp()
            {
                Coerce(unit) = static_cast<Unit *>( Y_STATIC_ZARR(wksp) );
                Coerce(nyt)  = unit + Unit::NYT;
                Coerce(eos)  = unit + Unit::EOS;
                Y_STATIC_ZARR(ctrl);

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
                nchr = 0;
                emit = & Alphabet::Init;
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


            void Alphabet:: write(StreamBits &io, const uint8_t byte)
            {
                assert(0!=emit);
                ( (*this).*emit )(io,byte);
                while(sumf>1000)
                {
                    reduceFrequencies();
                }
            }


            void Alphabet:: Init(StreamBits &io, const uint8_t byte)
            {
                assert(0==sumf);
                assert(0==nchr);
                Unit &u = unit[byte]; assert(0==u.freq);
                u.freq++;
                sumf++;
                nchr++;
                u.to(io);
                emit = & Alphabet::Bulk;
                used.pushHead(&u);
            }


            static inline void Rank(ListOf<Unit> &used, Unit * const node)
            {
                assert(0!=node);
                const uint32_t freq = node->freq;
            CHECK:
                const Unit *   prev = node->prev;
                if(prev && prev->freq < freq )
                {
                    used.towardsHead(node);
                    goto CHECK;
                }
            }

            void Alphabet:: Bulk(StreamBits &io, const uint8_t byte)
            {
                assert(sumf>0);
                assert(nchr>0);
                assert(nchr<Unit::Encoding);
                assert(used.owns(nyt));

                Unit &u = unit[byte];
                sumf++;
                if(u.freq++<=0)
                {
                    nyt->to(io);               // new char: signal NYT
                    used.insertBefore(nyt,&u); // insert befor NYT

                    if(++nchr>=Unit::Encoding) // check status
                    {
                        used.pop(nyt)->reset();   // all are transmitted
                        emit = & Alphabet:: Full; // change emit method
                    }
                }
                else
                    Rank(used,&u); // update status


                u.to(io);
                
            }

            void Alphabet:: Full(StreamBits &io, const uint8_t byte)
            {
                assert(nchr==Unit::Encoding);
                Unit &u = unit[byte]; assert(u.freq>0);
                u.freq++;
                sumf++;
                Rank(used,&u);
                u.to(io);
            }
        }

    }

}
