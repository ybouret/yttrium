
#include "y/chemical/weasel/compiler.hpp"
#include "y/chemical/plexus/injector.hpp"
#include "y/stream/libc/output.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/chemical/plexus/solver.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        //! a boundary is a list a species that vanish at given extent
        class Boundary : public SRepo
        {
        public:
            explicit Boundary(const SBank    &sb) noexcept : SRepo(sb), xi(0) {}
            Boundary(const Boundary &other) : SRepo(other), xi(other.xi) {}
            virtual ~Boundary() noexcept {}

            friend std::ostream & operator<<(std::ostream &os, const Boundary &self)
            {
                const SRepo &sr = self;
                if(sr.size>0)
                {
                    os << real_t(self.xi) << " @" << sr;
                }
                else
                {
                    os << "{none}";
                }
                return os;
            }

            void reset() noexcept { xi=0; free(); }

            xreal_t xi;

            static inline xreal_t getExtent(const Actor &a, const XReadable &C, const Level L) noexcept
            {
                const xreal_t zero;
                const Species &s = a.sp; assert(C.size()>=s.indx[L]);
                const xreal_t  c = C[s.indx[L]];
                return Max(c,zero)/a.xn;
            }

            Boundary & probe(const Actors &ac, const XReadable &C, const Level L)
            {
                Boundary     &self = *this;
                const xreal_t zero;
                reset();
                const Actor *a = ac->head;
                if(0!=a)
                {
                    // initialize with first actor
                    xi = getExtent(*a,C,L);
                    self << a->sp;

                    // upgrade with extra actors
                    for(a=a->next;a;a=a->next)
                    {
                        const xreal_t x = getExtent(*a,C,L);
                        switch(Sign::Of(x,xi))
                        {
                            case __Zero__: self << a->sp; break;               // same extent, add to slis
                            case Positive: break;                          // greater than xi, discard
                            case Negative: free(); xi=x; self << a->sp; break; // new winner
                        }
                    }
                }


                return self;
            }

            void neg() noexcept
            {
                const xreal_t zero;
                if(xi>zero) xi = -xi;
            }


        private:
            Y_DISABLE_ASSIGN(Boundary);
        };

        class Boundaries
        {
        public:
            explicit Boundaries(const SBank &sb) noexcept : forward(sb), reverse(sb) {}
            virtual ~Boundaries() noexcept {}
            Boundaries(const Boundaries &other) : forward(other.forward), reverse(other.reverse) {}

            void reset() noexcept { forward.reset(); reverse.reset(); }

            void probe(const Components &eq, const XReadable &C, const Level L)
            {
                forward.probe(eq.reac,C,L);
                reverse.probe(eq.prod,C,L).neg();
            }

            friend std::ostream & operator<<(std::ostream &os, const Boundaries &self)
            {
                os << "rev: " << self.reverse << " | fwd: " << self.forward;
                return os;
            }

            Boundary forward;
            Boundary reverse;

        private:
            Y_DISABLE_ASSIGN(Boundaries);
        };


#if 0
        class Solver
        {
        public:

            class Hint
            {
            public:
                Hint(const xreal_t      _xi,
                     const xreal_t      _sl,
                     const Equilibrium &_eq,
                     const XReadable   &Ceq) :
                xi(_xi),
                ax(xi.abs()),
                sl(_sl),
                eq(_eq),
                cc(Ceq)
                {
                }

                Hint(const Hint &_) noexcept :
                xi(_.xi),
                ax(_.ax),
                sl(_.sl),
                eq(_.eq),
                cc(_.cc)
                {
                }


                ~Hint() noexcept {}

                static inline int Compare(const Hint &lhs, const Hint &rhs) noexcept
                {
                    return Comparison::Decreasing(lhs.ax, rhs.ax);
                }

                const xreal_t      xi;
                const xreal_t      ax;
                const xreal_t      sl;
                const Equilibrium &eq;
                const XReadable   &cc;


            private:
                Y_DISABLE_ASSIGN(Hint);
            };


            explicit Solver(const Clusters &cls) :
            am(),
            ha(cls.maxEPC),
            Ceq(cls.maxEPC,cls.maxSPC),
            phi(cls.maxSPC,0),
            pC(0),
            tL(TopLevel),
            pK(0)
            {

            }

            virtual ~Solver() noexcept
            {
            }


            void process(const Cluster  & cl,
                         XWritable      & C,
                         const Level      L,
                         const XReadable &K,
                         XMLog           &xml)
            {
                Y_XML_SECTION_OPT(xml, "Solve::Cluster", " eqs='" << cl.size << "'");
                const xreal_t zero;

                ha.free();
                for(const ENode *en=cl.head;en;en=en->next)
                {
                    const Equilibrium &  eq  = **en;
                    const xreal_t        eK = K[eq.indx[TopLevel]];
                    const size_t         jj = ha.size()+1;
                    XWritable          & Cj = Ceq[jj];
                    if( am.solve(Cj, SubLevel, C, L, eq, eK) )
                    {
                        eq.drvsMassAction(eK, phi, SubLevel, Cj, SubLevel, am.xmul);
                        const xreal_t slope = eq.dot(phi, SubLevel, am.xadd);
                        const Hint    hint( am.eval(Cj, SubLevel, C, L, eq), slope, eq, Cj );
                        ha << hint;
                        if(false&&xml.verbose)
                        {
                            cl.uuid.pad(xml() << "+ " <<  eq.name, eq) << ":";
                            *xml << std::setw(15) << real_t(hint.xi) << " @";
                            eq.displayCompact(*xml, Cj, SubLevel);
                            *xml <<   std::endl;
                        }
                    }
                    
                }



                HeapSort::Call(ha,Hint::Compare);
                const size_t         nh = ha.size();
                LinearlyIndependent  li(cl.size,cl.species.size);

                li.init();
                for(size_t i=1;i<=nh;++i)
                {
                    const Hint &hint = ha[i];
                    if(xml.verbose)
                    {
                        cl.uuid.pad(xml() << "+ " << hint.eq.name, hint.eq);
                        *xml << " : xi=" << std::setw(15) << real_t(hint.xi);
                        *xml << " | sl=" << std::setw(15) << real_t(hint.sl);
                        hint.eq.displayCompact(*xml << " @", hint.cc, SubLevel);
                        *xml << std::endl;
                    }
                    if(li.keep(hint.eq,cl.topology))
                    {

                    }
                }

                Y_XMLOG(xml,"family: " << li);


            }


            Aftermath       am;
            CxxSeries<Hint> ha;
            XMatrix         Ceq;
            XVector         phi;

            const XReadable * pC;
            Level             tL;
            const XReadable  *pK;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Solver);
        };
#endif
        
    }

}

using namespace Yttrium;
using namespace Chemical;

Y_UTEST(solve)
{
    Weasel::Compiler &weasel = Weasel::Compiler::Instance();
    Library            lib;
    LuaEquilibria      eqs;
    Random::ParkMiller ran;

    for(int i=1;i<argc;++i)
    {
        weasel(lib,eqs,Jive::Module::OpenData(argv[i],argv[i]));
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    bool     verbose = true;
    XMLog    xml(verbose);
    Clusters clusters(eqs,xml);
    (void) clusters.K(0);

    clusters.showK(std::cerr);

    const size_t M = lib->size();
    XVector      C0(M,0);
    Species::Conc(C0,ran,0.3);

    if(false)
    {
        C0.ld(0);
        const Species *sp = lib.query("EtCOO-");
        if(sp)
        {
            C0[sp->indx[TopLevel]] = 1;
        }
    }
    lib(std::cerr,"[",C0,"]");



    SBank            sb;
    Boundaries       bnd(sb);
    Solver           solve(clusters);
    const XReadable &K = clusters.K(0);

    for(const Cluster *cl=clusters->head;cl;cl=cl->next)
    {
        if(false)
        {
            for(const ENode *en=cl->head;en;en=en->next)
            {
                const Equilibrium &eq = **en;
                bnd.probe(eq,C0,TopLevel);
                std::cerr << eq << ": " << bnd << std::endl;
            }
        }

        solve.process(C0,*cl,K,xml);


    }

    Y_SIZEOF(Components);
    Y_SIZEOF(Equilibrium);
    Y_SIZEOF(LinearlyIndependent);
    Y_SIZEOF(Cluster);

}
Y_UDONE()
