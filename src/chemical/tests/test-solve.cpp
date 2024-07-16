
#include "y/chemical/weasel/compiler.hpp"
#include "y/chemical/plexus/injector.hpp"
#include "y/stream/libc/output.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"


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
            explicit Solver(const Clusters &cls) :
            Ceq(cls.maxEPC,cls.maxSPC),
            dCe(cls.maxEPC,cls.maxSPC)
            {

            }

            virtual ~Solver() noexcept
            {
            }

            XMatrix Ceq;
            XMatrix dCe;

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

    lib(std::cerr,"[",C0,"]");

    SBank      sb;
    Boundaries bnd(sb);
    for(const Cluster *cl=clusters->head;cl;cl=cl->next)
    {
        for(const ENode *en=cl->head;en;en=en->next)
        {
            const Equilibrium &eq = **en;
            bnd.probe(eq,C0,TopLevel);
            std::cerr << eq << ": " << bnd << std::endl;
        }
    }


}
Y_UDONE()
