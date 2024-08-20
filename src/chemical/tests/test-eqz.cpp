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

            //! setup empty
            explicit Boundary(const SBank &_) noexcept :
            SRepo(_),
            xi(0)
            {
            }

            //! setup with initial value/speice
            explicit Boundary(const SBank   & b,
                              const xreal_t   x,
                              const Species & s)   :
            SRepo(b),
            xi(x)
            {
                (*this) << s;
            }

            //! duplicate
            explicit Boundary(const Boundary &other) :
            SRepo(other),
            xi(other.xi)
            {

            }


            //! first/update x>=0
            void operator()(const xreal_t x,
                            const Species &s)
            {
                try
                {
                    if(size<=0)
                    {
                        first(x,s);
                    }
                    else
                    {
                        assert(x>=0.0);
                        switch( Sign::Of(x,xi) )
                        {
                            case Negative: // new winner
                                free();
                                first(x,s);
                                break;

                            case __Zero__: // same value
                                (*this) << s;
                                break;

                            case Positive: // discard
                                break;
                        }
                    }
                }
                catch(...)
                {
                    empty();
                    throw;
                }
            }

            virtual ~Boundary() noexcept {}

            void empty() noexcept
            {
                free();
                xi = 0;
            }

            void first(const xreal_t x, const Species &s)
            {
                assert(0==size);
                (*this) << s;
                xi = x;
            }

            friend std::ostream & operator<<(std::ostream &os, const Boundary &B)
            {
                const SRepo &repo = B;
                if(repo.size>0)
                {
                    os << repo << "@" << real_t(B.xi);
                }
                else
                {
                    os << "none";
                }
                return os;
            }

            xreal_t xi;

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
            limiting(banks.s),
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

                // dispatch required, build limiting
                for(const Actor *a=A->head;a;a=a->next)
                {
                    const Species &sp = a->sp; if( !conserved.has(sp) ) continue;
                    const xreal_t  cc = C[ sp.indx[L] ];
                    if(cc.mantissa<0)
                    {
                        required((-cc)/a->xn,sp);
                        assert(required.sorted());
                    }
                    else
                    {
                        limiting(cc/a->xn,sp);
                    }
                }

                // return if bad
                return required.size>0;
            }

            friend std::ostream & operator<<(std::ostream &os, const Fader &f)
            {
                os << "lim="  << f.limiting;
                os << "/req=" << f.required;
                return os;
            }

            Boundary   limiting;
            Boundaries required;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Fader);
        };

        class Faders : public Recyclable
        {
        public:
            static const unsigned BALANCED = 0x00;
            static const unsigned BAD_REAC = 0x01;
            static const unsigned BAD_PROD = 0x02;
            static const unsigned BAD_BOTH = BAD_REAC | BAD_PROD;

            typedef CxxArray<Faders,XMemory> Array;
            static const char * TextFlag(const unsigned flag) noexcept
            {
                switch(flag)
                {
                    case BALANCED: return "BALANCED";
                    case BAD_REAC: return "BAD REAC";
                    case BAD_PROD: return "BAD PROD";
                    case BAD_BOTH: return "BAD BOTH";
                    default:
                        break;
                }
                return Core::Unknown;
            }

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

            unsigned operator()(const XReadable   &C,
                                const Level       &L,
                                const Components  &E,
                                const AddressBook &conserved)
            {
                free();
                try
                {
                    unsigned flag = BALANCED;
                    if(reac(C,L,E.reac,conserved)) flag |= BAD_REAC;
                    if(prod(C,L,E.prod,conserved)) flag |= BAD_PROD;
                    return flag;
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


        class Altered
        {
        public:
            typedef CxxSeries<Altered,XMemory> Series;

            explicit Altered(const XReadable & _cc) noexcept :
            cc(_cc)
            {
            }

            Altered(const Altered &_) noexcept :
            cc(_.cc)
            {
            }


            ~Altered() noexcept {}

            const XReadable &cc;


        private:
            Y_DISABLE_ASSIGN(Altered);
        };

        class Equalizer
        {
        public:
            explicit Equalizer(const Cluster &cl) :
            rcl(cl),
            neq(cl.size),
            nsp(cl.species.size),
            banks(),
            best(banks.s),
            faders(neq,CopyOf,banks),
            ceq(neq,nsp),
            altered(neq)
            {
            }

            virtual ~Equalizer() noexcept {}


            void run(XWritable &C, const Level L, XMLog &xml)
            {
                Y_XML_SECTION(xml, "Eqz");
                const AddressBook &conserved = rcl.conserved.book;

                altered.free();
                for(const ENode *en=rcl.limited.head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    Faders            &fd = faders[ eq.indx[SubLevel] ];
                    const unsigned     id = fd(C,L,eq,conserved);
                    const size_t       ii = altered.size() + 1;
                    XWritable         &cc = ceq[ii];
                    if(xml.verbose)
                    {
                        rcl.uuid.pad( xml() << Faders::TextFlag(id) << " | " << eq.name, eq) << std::endl;
                        if(id!=Faders::BALANCED)
                        {
                            xml() << "\treac: " << fd.reac << std::endl;
                            xml() << "\tprod: " << fd.prod << std::endl;
                        }
                    }

                    switch(id)
                    {
                        case Faders::BALANCED:
                            continue;

                        case Faders::BAD_BOTH:
                            altered << Altered( rcl.transfer(cc,SubLevel,C,L) );
                            continue;

                        case Faders::BAD_PROD: {
                            // need a forward alteration
                            assert(fd.prod.required.size>0);
                            assert(fd.reac.limiting.size>0);
                            if(bestEffort(fd.reac.limiting,fd.prod.required))
                            {
                                Y_XMLOG(xml, "\tbest: " << best);
                            }
                            else
                            {
                                Y_XMLOG(xml, "\tno best effort");
                            }
                        } continue;

                        case Faders::BAD_REAC: {
                            // need a reverse alteration
                            assert(fd.reac.required.size>0);
                            assert(fd.prod.limiting.size>0);
                            if(bestEffort(fd.prod.limiting,fd.reac.required))
                            {
                                best.xi = -best.xi;
                                Y_XMLOG(xml, "\tbest: " << best);
                            }
                            else
                            {
                                Y_XMLOG(xml, "\tno best effort");
                            }
                        }  continue;

                    }
                    throw Exception("bad id");
                }
            }

            bool bestEffort(const Boundary   &limiting,
                            const Boundaries &required)
            {
                best.empty();
                assert(limiting.size>0); assert(limiting.xi>=0.0);
                assert(required.size>0);

                const xreal_t    ximax = limiting.xi;
                const Boundary  *lower = 0;

                for(const BNode *node  = required.head;node;node=node->next)
                {
                    const Boundary &here = **node;
                    switch(Sign::Of(here.xi,ximax))
                    {
                        case Negative:
                            lower = &here; // record for later
                            continue;

                        case __Zero__:
                            best.xi = ximax;   // numerical match
                            best << here;      // load vanishing in here
                            best << limiting;  // load vanishing in limiting
                            return true;

                        case Positive:
                            goto DONE; // no more possibility

                    }
                }

            DONE:
                if(0 != lower)
                {
                    const Boundary &here = *lower;
                    best.xi = here.xi;
                    best << here;
                    return true;
                }
                else
                {
                    if(ximax>0.0)
                    {
                        best.xi = ximax;
                        best << limiting;
                        return true;
                    }
                    else
                        return false;
                }
            }

            const Cluster &   rcl;
            const size_t      neq;
            const size_t      nsp;
            Boundaries::Banks banks;
            Boundary          best;
            Faders::Array     faders;
            XMatrix           ceq;
            Altered::Series   altered;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equalizer);
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
        Equalizer eqz(*cl);

        plexus.conc(C0,0.3,0.5);
        lib(std::cerr << "C0=","\t[",C0,"]");

        eqz.run(C0, TopLevel, plexus.xml);

#if 0
        for(const ENode *en=cl->head;en;en=en->next)
        {
            const Equilibrium &eq = **en;
            F(C0,TopLevel,eq,cl->conserved.book);
            cl->uuid.pad(std::cerr << eq.name, eq) << ":" << F << std::endl;
        }
#endif

        //lib(std::cerr << "C1=","\t[",C0,"]");



    }




    Y_SIZEOF(Boundary);
    Y_SIZEOF(Boundaries);
}
Y_UDONE()
