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

            class Banks
            {
            public:
                Banks() : b(), s() {}
                ~Banks() noexcept {}
                BBank b;
                SBank s;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Banks);
            };


            explicit Boundaries(const Banks &banks) noexcept :
            BList(banks.b),
            sbank(banks.s)
            {
            }

            virtual ~Boundaries() noexcept {}

            //! insert vanishing extent and its species
            void operator()(const xreal_t         xi,
                            const Species &       sp)
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

            bool sorted() const noexcept
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




        private:
            const SBank sbank;
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


        //! fader for actors
        class Fader : public Recyclable
        {
        public:
            explicit Fader(const Boundaries::Banks &banks) noexcept:
            limiting(banks),
            required(banks)
            {
            }

            virtual ~Fader() noexcept {}

            virtual void free() noexcept { limiting.free(); required.free(); }

            bool operator()(const XReadable  &C,
                            const Level      &L,
                            const Actors     &A,
                            const AddressBook &conserved)
            {
                assert(0==limiting.size);
                assert(0==required.size);

                // dispatch all extents
                for(const Actor *a=A->head;a;a=a->next)
                {
                    const Species &sp = a->sp; if( !conserved.has(sp) ) continue;
                    const xreal_t  cc = C[ sp.indx[L] ];
                    if(cc.mantissa<0)
                    {
                        required((-cc)/a->xn,sp);
                    }
                    else
                    {
                        limiting(cc/a->xn,sp);
                    }
                }

                // keep limiting extent if any
                while(limiting.size>1)
                    limiting.cutTail();

                return required.size>0;
            }

            friend std::ostream & operator<<(std::ostream &os, const Fader &f)
            {
                os << "lim=" << f.limiting;
                os << "/req=" << f.required;
                return os;
            }

            Boundaries limiting;
            Boundaries required;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Fader);
        };

        class Faders : public Recyclable
        {
        public:
            explicit Faders(const Boundaries::Banks &banks) noexcept :
            reac(banks),
            prod(banks)
            {
            }

            virtual ~Faders() noexcept {}

            virtual void free() noexcept { reac.free(); prod.free(); }

            friend std::ostream & operator<<(std::ostream &os, const Faders &F)
            {
                os << "{reac:" << F.reac << "|prod:" << F.prod << "}";
                return os;
            }

            void operator()(const XReadable   &C,
                            const Level       &L,
                            const Components  &E,
                            const AddressBook &conserved)
            {
                free();
                try
                {
                    static unsigned BALANCED = 0x00;
                    static unsigned BAD_REAC = 0x01;
                    static unsigned BAD_PROD = 0x02;
                    static unsigned BAD_BOTH = BAD_REAC | BAD_PROD;
                    unsigned flag = BALANCED;
                    if(reac(C,L,E.reac,conserved)) flag |= BAD_REAC;
                    if(prod(C,L,E.prod,conserved)) flag |= BAD_PROD;
                    switch(flag)
                    {
                        case BALANCED:
                            break;
                        case BAD_REAC:
                            break;
                        case BAD_PROD:
                            break;

                        case BAD_BOTH:
                            break;
                    }
                }
                catch(...)
                {
                    free();
                    throw;
                }
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

    Boundaries::Banks banks;
    Faders            F(banks);
    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {
        //Equalizer eqz(*cl);

        plexus.conc(C0,0.3,0.5);
        lib(std::cerr << "C0=","\t[",C0,"]");

        for(const ENode *en=cl->head;en;en=en->next)
        {
            const Equilibrium &eq = **en;
            F(C0,TopLevel,eq,cl->conserved.book);
            cl->uuid.pad(std::cerr << eq.name, eq) << ":" << F << std::endl;
        }

        //lib(std::cerr << "C1=","\t[",C0,"]");



    }




    Y_SIZEOF(Boundary);
    Y_SIZEOF(Boundaries);
}
Y_UDONE()
