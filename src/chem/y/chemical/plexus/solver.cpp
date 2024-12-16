
#include "y/chemical/plexus/solver.hpp"
#include "y/stream/libc/output.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Solver:: ~Solver() noexcept {}

        Solver:: Solver(const Mix &_) :
        mix(_),
        my(),
        Csolve(mix->size,mix->species.size),
        deltaC(mix->size,mix->species.size),
        Cini(mix->species.size),
        Cend(mix->species.size),
        Capp(mix->species.size),
        Ctmp(mix->species.size),
        step(mix->species.size),
        Cadd(mix->species.size),
        one(1)
        {

        }

        const char * const Solver:: CallSign = "Chemical::Solver";
        const char * const Solver:: ODE      = "ODE";

        Solver::ConstInterface & Solver:: surrogate() const noexcept { return my; }


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

    }

}
