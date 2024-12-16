
#include "y/chemical/plexus/solver.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Solver:: ~Solver() noexcept {}

        Solver:: Solver(const Mix &_) :
        mix(_),
        Csolve(mix->size,mix->species.size),
        deltaC(mix->size,mix->species.size),
        Cini(mix->species.size),
        Cend(mix->species.size),
        Ctmp(mix->species.size),
        pbank(),
        plist(pbank)
        {

        }

        const char * const Solver:: CallSign = "Chemical::Solver";


      
        void Solver:: update() noexcept
        {
            forget();
            for(const ProNode *pn=plist.head;pn;pn=pn->next)
                enroll(**pn);
            
        }

        xReal Solver:: objectiveFunction(const XReadable &C, const Level L)
        {
            const xReal den = plist.size;
            xadd.free();
            for(const ProNode *pn=plist.head;pn;pn=pn->next)
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
            const xReal  _1 = 1;
            const xReal   v = _1 - u;
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



        void Solver:: show(XMLog &xml) const
        {
            Y_XML_SECTION_OPT(xml, "Chemical::Prospects", "count='" << plist.size << "'");
            for(const ProNode *pn=plist.head;pn;pn=pn->next)
            {
                const Prospect &pro = **pn;
                if(xml.verbose) {
                    const char * const info = Components::SituationText(pro.out.st);
                    print(xml() << info << " @",pro,Justify::Left)
                    << " : |xi| = | " << std::setw(15) << real_t(pro.xi) << " |"
                    << " ff = " << std::setw(15) << real_t(pro.ff)
                    << std::endl;
                }
            }
        }


    }

}
