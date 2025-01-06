
#include "y/chemical/plexus/solver.hpp"
#include "y/stream/libc/output.hpp"
#include "y/chemical/plexus/mix.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Solver:: ~Solver() noexcept {}

        Solver:: Solver(const Mix &    _mx,
                        const XBanks & xbk) :
        mix(_mx),
        my(),
        Csolve(mix->size,mix->species.size),
        deltaC(mix->size,mix->species.size),
        Cini(mix->species.size),
        Cend(mix->species.size),
        Ctmp(mix->species.size),
        step(mix->species.size),
        Cerr(mix->species.size),
        Cadd(mix->species.size),
        ortho(mix->species.size,mix->size),
        basis( static_cast<ProBank &>(my) ),
        lu(mix->size),
        xiArr(  mix->topology.size() ),
        PhiMat( mix->topology.size() ),
        NuMat(  mix->topology.size() ),
        NuTMat( mix->topology.size() ),
        XiMat(  mix->topology.size() ),
        crs(xbk),
        one(1),
        safe(0.99)
        {

            const size_t m = mix->species.size;
            for(size_t n=1;n<=mix->topology.size();++n)
            {
                xiArr.  grow(n);
                PhiMat. grow(n,m);
                NuMat.  grow(n,m);
                NuTMat. grow(m,n);
                XiMat.  grow(n,n);
            }

        }

        const char * const Solver:: CallSign = "Chemical::Solver";

        const char * const Solver:: OptimalProspect = "OptimalProspect";
        const char * const Solver:: DynamicalSystem = "DynamicalSystem";
        const char * const Solver:: AlgebraicSystem = "AlgebraicSystem";


        Y_PROXY_IMPL(Solver,my)

        

        xReal Solver:: objectiveFunction(const XReadable &C, const Level L)
        {
            const xReal den = my.size;
            xadd.free();
            for(const ProNode *pn=my.head;pn;pn=pn->next)
            {
                const Prospect &pro = **pn;
                const xReal     score = pro.score(xmul, C, L);
                const xReal     sc2   = score * score;
                xadd << sc2;
            }

            return (xadd.sum()/den).sqrt();
        }


        xReal Solver:: objectiveFunction(const xReal u)
        {
            const xReal   v = one - u;
            const size_t  m = Ctmp.size();
            for(size_t j=m;j>0;--j)
            {
                const xReal c0 = Cini[j];
                const xReal c1 = Cend[j];
                xReal       cmin = c0;
                xReal       cmax = c1;
                if(cmax<cmin) Swap(cmin,cmax);
                assert(cmin<=cmax);
                const xReal ctmp = c0 * v + c1 * u;
                Ctmp[j] = Clamp(cmin, ctmp, cmax);
            }

            return objectiveFunction(Ctmp,SubLevel);
        }

        xReal Solver:: operator()(const xReal u) { return objectiveFunction(u); }


        void Solver:: save(OutputStream &fp, const size_t np) 
        {
            for(size_t i=0;i<=np;++i)
            {
                const double u = double(i)/np;
                const double f = double(objectiveFunction(u));
                fp("%.15g %.15g\n",u,f);
            }
        }


        void Solver:: save(const String &fn, const size_t np)
        {
            OutputFile fp(fn);
            save(fp,np);
        }


        void Solver:: savePro(const String &fn, const size_t np)
        {
            OutputFile fp(fn);
            for(const ProNode *pn=my.head;pn;pn=pn->next)
            {
                const Prospect &pro = **pn;
                mix.transfer(Cend, SubLevel, pro.C, pro.L);
                save(fp,np);
                fp << "\n";
            }
        }

        void Solver:: setRecentStep(const xReal scal) noexcept
        {
            assert( Cini.size() == step.size() );
            assert( Cend.size() == step.size() );
            for(size_t i=Cend.size();i>0;--i)
            {
                Cend[i] = Cini[i] + scal * step[i];
            }
        }

        bool Solver:: isAcceptable(const XReadable &C, const Level L) const noexcept
        {

            for(const ProNode *pn=my.head;pn;pn=pn->next)
            {
                const Components &cm = (**pn).eq;
                switch(cm.situation(C,L))
                {
                    case Running: continue;
                    case Blocked:
                    case Crucial:
                        return false;
                }
            }
            return true;
        }

    }

}
