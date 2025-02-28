

#include "y/chemical/plexus/mixes.hpp"

#include "y/chemical/library.hpp"
#include "y/random/park-miller.hpp"



#include "y/utest/run.hpp"
#include "y/stream/libc/output.hpp"

namespace Yttrium
{

    namespace Chemical
    {

        //! request are ordered extent to correct negative values
        class Requests : public Cursors
        {
        public:
            explicit Requests(const XBanks &_) noexcept : Cursors(_) {}
            virtual ~Requests() noexcept {}
            Requests(const Requests &_) : Cursors(_) {}

        private:
            Y_DISABLE_ASSIGN(Requests);
        };


        //! limiting extent due to positive or zero value
        class Limiting : public Marker
        {
        public:
            explicit Limiting(const XBanks &_) : Marker(_.species) {}
            explicit Limiting(const Limiting &_) : Marker(_) {}
            virtual ~Limiting() noexcept {}

            void operator()(const Species &sp, const xReal xi)
            {
                assert(xi>=0.0);
                if(srepo.size<=0)
                {
                    srepo << sp;
                    value = xi;
                }
                else
                {
                    switch( Sign::Of(xi,value) )
                    {
                        case Negative:
                            // new winner
                            srepo.free();
                            srepo << sp;
                            value = xi;
                            break;

                        case __Zero__:
                            // group same extent
                            srepo << sp;
                            break;


                        case Positive:
                            // discard
                            break;
                    }
                }
            }


        private:
            Y_DISABLE_ASSIGN(Limiting);

        };


        class Extents
        {
        public:
            Extents(const Actors      &actors,
                    const XReadable   &C,
                    const Level        L,
                    const XBanks      &xbanks,
                    const AddressBook &conserved) noexcept :
            limiting(xbanks),
            requests(xbanks)
            {
                for(const Actor *a=actors->head;a;a=a->next)
                {
                    const Species &sp = a->sp;   if(!conserved.has(sp)) continue;
                    const xReal    cc = sp(C,L);
                    const xReal    xi = cc / a->xn;
                    if(xi>=0.0)
                    {
                        limiting(sp,xi);
                    }
                    else
                    {
                        requests(sp,xi);
                    }
                }
            }

            ~Extents() noexcept {}
            Extents(const Extents &_) : limiting(_.limiting), requests(_.requests) {}

            Y_OSTREAM_PROTO(Extents);


            Limiting limiting; //!< limiting extent   if c>=0
            Requests requests; //!< requested extents if c<0



        private:
            Y_DISABLE_ASSIGN(Extents);
        };

        std::ostream & operator<<(std::ostream &os, const Extents &ex)
        {
            os << "lim@";
            if(ex.limiting->size>0)
            {
                os << ex.limiting;
            }
            else
            {
                os << Core::Ptr::Nil;
            }
            os << ",req@";
            if(ex.requests->size>0)
            {
                os << ex.requests;
            }
            else
            {
                os << Core::Ptr::Nil;
            }
            return os;
        }

        //! boundaries from reactants and products
        class Frontiers
        {
        public:
            Frontiers(const Components &eq,
                      const XReadable  &C,
                      const Level       L,
                      const XBanks      &xbanks,
                      const AddressBook &conserved) :
            reac(eq.reac,C,L,xbanks,conserved),
            prod(eq.prod,C,L,xbanks,conserved),
            good(reac.requests->size<=0 && prod.requests->size<=0)
            {
            }


            ~Frontiers() noexcept {}

            Frontiers(const Frontiers &_) :
            reac(_.reac),
            prod(_.prod),
            good(_.good)
            {
            }

            Y_OSTREAM_PROTO(Frontiers);

            const Extents  reac;
            const Extents  prod;
            const bool     good;

        private:
            Y_DISABLE_ASSIGN(Frontiers);
        };


        std::ostream & operator<<(std::ostream &os, const Frontiers &f)
        {
            os << (f.good? "(+) " : "(-) ") << "reac={" << f.reac << "} prod={" << f.prod << "}";
            return os;
        }

        

        class Equalizer
        {
        public:

            explicit Equalizer(const Mix    &_mix,
                               const XBanks &_xbanks) :
            mix(_mix),
            xbanks(_xbanks)
            {
            }

            virtual ~Equalizer() noexcept
            {
            }


            const Mix &mix;
            XBanks     xbanks;

            void run(XMLog &xml, XWritable &C, const Level L)
            {
                const AddressBook &conserved = mix.genus->conserved.book;
                for(const ENode *en=mix.grade->limiting.list.head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    const Frontiers    ff(eq,C,L,xbanks,conserved);
                    (**mix).print(std::cerr,eq,Justify::Right) << ": " << ff << std::endl;

                }
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equalizer);
        };


    }

}

using namespace Yttrium;
using namespace Chemical;


Y_UTEST(eqz)
{
    Random::ParkMiller ran;
    Library            lib;
    Equilibria         eqs;

    for(int i=1;i<argc;++i)
    {
        eqs(lib,argv[i]);
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    bool     verbose = true;
    XMLog    xml(verbose);
    Mixes    mixes(xml,eqs);

    std::cerr << "mixes=" << mixes << std::endl;
    GraphViz::Vizible::DotToPngEx("warden.dot",mixes,1);

    const size_t M = lib->size();
    XVector      C0(M,0);
    XVector      dC(M,0);

    Library::Conc(C0,ran,0.5,0.5);

    lib(std::cerr << "C0=","[",C0,"]", xReal::ToString) << std::endl << std::endl;

    mixes.guard(xml, C0, TopLevel, dC);
    for(const Mix *mix=mixes->head;mix;mix=mix->next)
    {
        Equalizer eqz(*mix,mixes.xbanks);

        const AddressBook &conserved = mix->genus->conserved.book;
        std::cerr << "limiting=" << mix->grade->limiting.list << std::endl;
        eqz.run(xml, C0, TopLevel);

        if(false)
            for(const ENode *en=mix->grade->limiting.list.head;en;en=en->next)
            {
                const Equilibrium &eq = **en;
                const Frontiers bnd(eq,C0,TopLevel,mixes.xbanks,conserved);
                (***mix).print(std::cerr,eq,Justify::Right) << ": " << bnd << std::endl;
                const Frontiers cpy(bnd);
                (***mix).print(std::cerr,eq,Justify::Right) << ": " << cpy << std::endl;

            }
    }

    Y_SIZEOF(Requests);
    Y_SIZEOF(Limiting);
    Y_SIZEOF(Extents);
    Y_SIZEOF(Frontiers);


}
Y_UDONE()
