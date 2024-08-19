#include "y/chemical/plexus.hpp"
#include "y/utest/run.hpp"
#include "y/data/small/heavy/list/coop.hpp"

using namespace Yttrium;
using namespace Chemical;

namespace Yttrium
{
    namespace Chemical
    {

        //! boundary extent and vanishing species
        class Boundary : public SRepo
        {
        public:
            explicit Boundary(const SBank   & b,
                              const xreal_t   x,
                              const Species & s)   :
            SRepo(b),
            xi(x)
            {
                (*this) << s;
            }

            explicit Boundary(const Boundary &other) :
            SRepo(other),
            xi(other.xi)
            {

            }

            virtual ~Boundary() noexcept {}

            const xreal_t xi;

        private:
            Y_DISABLE_ASSIGN(Boundary);
        };

        typedef Small::CoopHeavyList<Boundary> BList;
        typedef BList::NodeType                BNode;
        typedef BList::ProxyType               BBank;

        //! ordered boundaries
        class Boundaries : public BList
        {
        public:
            explicit Boundaries(const BBank &bb, const SBank &sb) noexcept :
            BList(bb),
            sbank(sb)
            {
            }

            virtual ~Boundaries() noexcept {}

            void add(const xreal_t  xi,
                     const Species &sp)
            {
                switch(size)
                {
                    case 0: atTail(xi,sp); return;

                    case 1:
                        switch( Sign::Of(xi,(**head).xi) )
                        {
                            case Negative: atHead(xi,sp);  break;
                            case Positive: atTail(xi,sp);  break;
                            case __Zero__: (**head) << sp; break;
                        }
                        return;

                    default:
                        break;
                }
                throw Exception("not implemented");
            }


            SBank sbank;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Boundaries);
            void atTail(const xreal_t xi, const Species &sp) {
                const Boundary b(sbank,xi,sp);
                (*this) << b;
            }

            void atHead(const xreal_t xi, const Species &sp) {
                const Boundary b(sbank,xi,sp);
                (*this) >> b;
            }
        };

        class Fader
        {
        public:
            Fader(const BBank &bbank, const SBank &sbank) noexcept :
            limiting(bbank,sbank),
            required(bbank,sbank)
            {
            }

            ~Fader() noexcept
            {
            }

            Boundaries limiting; //!< positive extents
            Boundaries required; //!< negative extents

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Fader);
        };

        class Faders
        {
        public:
            Faders(const BBank &bbank, const SBank &sbank) noexcept :
            reac(bbank,sbank),
            prod(bbank,sbank)
            {

            }

            ~Faders() noexcept
            {
            }



            Fader reac;
            Fader prod;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Faders);
        };



    }

}

Y_UTEST(eqz)
{
    Plexus plexus(true);

    for(int i=1;i<argc;++i)
    {
        plexus(Jive::Module::OpenData(argv[i],argv[i]));
    }

    std::cerr << "lib=" << plexus.lib << std::endl;
    std::cerr << "eqs=" << plexus.eqs << std::endl;

    const Library   &lib = plexus.lib;
    Clusters        &cls = plexus.assemble();
    //const XReadable &K   = cls.K(0);
    XVector C0(lib->size(),0);

    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {
        plexus.conc(C0,0.3,0.5);
    }
    

    Y_SIZEOF(Boundary);
    Y_SIZEOF(Boundaries);
    Y_SIZEOF(Fader);
    Y_SIZEOF(Faders);
}
Y_UDONE()
