
#include "y/chemical/weasel/compiler.hpp"
#include "y/chemical/plexus/injector.hpp"
#include "y/stream/libc/output.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/chemical/reactive/aftermath.hpp"
#include "y/sort/heap.hpp"

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


        class Solver
        {
        public:
            class Hint
            {
            public:
                Hint(const xreal_t     val,
                     const Components &com,
                     XWritable        &Ceq,
                     XWritable        &dCe) noexcept :
                xi(val),
                ax(xi.abs()),
                eq(com),
                cc(Ceq),
                dc(dCe)
                {
                }

                Hint(const Hint &_) noexcept :
                xi(_.xi),
                ax(_.ax),
                eq(_.eq),
                cc(_.cc),
                dc(_.dc)
                {
                }

                static int Compare(const Hint &lhs, const Hint &rhs) noexcept
                {
                    return Comparison::Decreasing(lhs.ax,rhs.ax);
                }

                const xreal_t     xi;
                const xreal_t     ax;
                const Components &eq;
                XWritable        &cc;
                XWritable        &dc;

            private:
                Y_DISABLE_ASSIGN(Hint);
            };

            explicit Solver(const Clusters &cls) :
            am(),
            hint(cls.maxEPC),
            Ceq(cls.maxEPC,cls.maxSPC),
            dCe(cls.maxEPC,cls.maxSPC)
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
                {
                    Y_XML_SECTION(xml, "Aftermath");
                    hint.free();
                    for(const ENode *en=cl.head;en;en=en->next)
                    {
                        const Equilibrium &  eq  = **en;
                        const xreal_t        eK = K[eq.indx[TopLevel]];
                        const size_t         jj = hint.size()+1;
                        XWritable          & Cj = Ceq[jj];
                        XWritable          & Dj = dCe[jj];
                        if( am.solve(Cj, SubLevel, C, L, eq, eK) )
                        {
                            const Hint clue( am.eval(Dj, Cj, SubLevel, C, L, eq), eq, Cj, Dj );
                            if(xml.verbose)
                            {
                                cl.uuid.pad(xml() << "+ " << eq,eq) << " : " << std::setw(15) << real_t(clue.xi);
                                eq.displayCompact(*xml << " @", Cj, SubLevel) << std::endl;
                            }
                            hint << clue;
                        }
                        else
                        {
                            // Y_XMLOG(xml," (-) " << eq);
                        }
                    }
                }
                {
                    Y_XML_SECTION(xml, "Hinting");
                    HeapSort::Call(hint,Hint::Compare);
                    const size_t nh = hint.size();
                    for(size_t i=1;i<=nh;++i)
                    {
                        if(xml.verbose)
                        {
                            const Hint &clue = hint[i];
                            cl.uuid.pad(xml() << "+ " << clue.eq.name, clue.eq) << " : " << std::setw(15) << real_t(clue.xi);
                            clue.eq.displayCompact(*xml << " d=", clue.dc, SubLevel);

                            *xml << std::endl;
                        }
                    }

                }


            }


            Aftermath       am;
            CxxSeries<Hint> hint;
            XMatrix         Ceq;
            XMatrix         dCe;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Solver);
        };
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
        for(const ENode *en=cl->head;en;en=en->next)
        {
            const Equilibrium &eq = **en;
            bnd.probe(eq,C0,TopLevel);
            std::cerr << eq << ": " << bnd << std::endl;
        }

        solve.process(*cl,C0,TopLevel,K,xml);


    }


}
Y_UDONE()
