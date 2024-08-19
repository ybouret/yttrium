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

            friend std::ostream & operator<<(std::ostream &os, const Boundary &B)
            {
                const SRepo &repo = B;
                os << repo << "@" << real_t(B.xi);
                return os;
            }

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

            void operator()(const xreal_t  xi,
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

                // check against head
                BNode *lower = head;
                switch( Sign::Of(xi, (**lower).xi) )
                {
                    case Negative: atHead(xi,sp);   return;
                    case __Zero__: (**lower) << sp; return;
                    case Positive:                  break;
                }

                // check againt tail
                BNode * const upper = tail; assert(upper!=lower);
                switch( Sign::Of(xi, (**upper).xi) )
                {
                    case Negative: break;
                    case __Zero__: (**upper) << sp; return;
                    case Positive: atTail(xi,sp);   return;

                }

            PROBE:
                BNode * const probe = lower->next;
                if(upper!=probe)
                {
                    switch( Sign::Of(xi, (**probe).xi) )
                    {
                        case Negative: goto FOUND;
                        case __Zero__: (**probe) << sp; return;
                        case Positive: break;
                    }
                    lower = probe;
                    goto PROBE;
                }

            FOUND:
                const Boundary b(sbank,xi,sp);
                insertAfter(lower,proxy->produce(b));
            }

            bool isSorted() const noexcept
            {
                if(size>1)
                {
                    size_t       num = size-1;
                    const BNode *lhs = head;      assert(0!=lhs);
                    const BNode *rhs = lhs->next; assert(0!=rhs);
                    while(num-- > 0)
                    {
                        assert(0!=lhs);
                        assert(0!=rhs);
                        switch(Sign::Of( (**lhs).xi, (**rhs).xi ) )
                        {
                            case Negative: break;
                            case __Zero__:
                            case Positive:
                                return false;
                        }
                        lhs = rhs;
                        rhs = lhs->next;
                    }
                }
                return true;
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

        //! for reac/prod
        class Fader : public Recyclable
        {
        public:
            Fader(const BBank &bbank, const SBank &sbank) noexcept :
            limiting(bbank,sbank),
            required(bbank,sbank)
            {
            }

            virtual ~Fader() noexcept
            {
            }

            virtual void free() noexcept
            {
                limiting.free();
                required.free();
            }

            void operator()(const XReadable  &C,
                            const Level       L,
                            const Actors     &A)
            {
                assert(0==limiting.size);
                assert(0==required.size);
                for(const Actor *a = A->head;a;a=a->next)
                {
                    const Species &s = a->sp;
                    const xreal_t  c = C[ s.indx[L] ];
                    const xreal_t  x = c/a->xn;
                    if(x.mantissa<0)
                    {
                        required(x,s);
                        assert(required.isSorted());
                    }
                    else
                    {
                        limiting(x,s);
                        assert(limiting.isSorted());
                    }
                }
            }

            friend std::ostream & operator<<(std::ostream &os, const Fader &F)
            {
                if(F.limiting.size)
                {
                    os << "lim=" << F.limiting;
                }
                else
                {
                    os << "[no limiting]";
                }
                if(F.required.size)
                {
                    os << "req=" << F.required;
                }
                else
                {
                    os << "[no required]";
                }
                return os;
            }


            Boundaries limiting; //!< positive extents
            Boundaries required; //!< negative extents

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Fader);
        };

        //! for both reac and prod
        class Faders : public Recyclable
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

            virtual void free() noexcept
            {

                reac.free();
                prod.free();
            }

            void operator()(const XReadable &C, const Level L, const Components &E)
            {
                free();
                try
                {
                    reac(C,L,E.reac);
                    prod(C,L,E.prod);
                }
                catch(...)
                {
                    free();
                    throw;
                }
            }

            friend std::ostream & operator<<(std::ostream &os, const Faders &F)
            {
                os << "{reac:" << F.reac;
                os << "|prod:" << F.prod;
                os << "}";
                return os;
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

    BBank  bbank;
    SBank  sbank;
    Faders faders(bbank,sbank);

    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {
        //plexus.conc(C0,0.3,0.5);


        for(const ENode *en=cl->head;en;en=en->next)
        {
            const Equilibrium &eq = **en;
            for(size_t iter=0;iter<100;++iter)
            {

                plexus.conc(C0);
                lib(std::cerr << "C0=","\t[",C0,"]");
                faders(C0,TopLevel,eq);
                cl->uuid.pad(std::cerr << eq, eq) << ":";
                std::cerr << faders;
                std::cerr << std::endl;
            }
        }
    }




    Y_SIZEOF(Boundary);
    Y_SIZEOF(Boundaries);
    Y_SIZEOF(Fader);
    Y_SIZEOF(Faders);
}
Y_UDONE()
