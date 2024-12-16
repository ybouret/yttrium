#include "y/chemical/plexus/solver.hpp"
#include "y/stream/libc/output.hpp"
#include "y/mkl/opt/minimize.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        void Solver:: makeStep(XMLog &xml, const xReal f0)
        {
            Y_XML_SECTION(xml, "makeStep");
            bool  hasCut = false;
            xReal factor = one;
            for(const SNode *sn=mix->species.head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                const xReal    cc = sp(Cini,SubLevel); assert(cc>=0.0);
                const xReal    dc = sp(step,SubLevel);
                if(dc<0.0)
                {
                    const xReal scale = cc/-dc;
                    factor = Min(factor,scale);
                    hasCut = true;
                }
            }
            Y_XMLOG(xml, "hasCut=" << hasCut);
            Y_XMLOG(xml, "factor=" << real_t(factor) );

            if(hasCut)
            {
                factor *= 0.9;

            }

            for(const SNode *sn=mix->species.head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                sp(Cend,SubLevel) = sp(Cini,SubLevel) + factor * sp(step,SubLevel);
                assert(sp(Cend,SubLevel) >= 0.0 );
            }

            XTriplet x = { zero, zero, one };
            XTriplet f = { f0,   f0,   objectiveFunction(Cend,SubLevel) };

            std::cerr << "ff=" << real_t(f.c) << std::endl;
            Solver &    F     = *this;
            const xReal x_opt = MKL::Minimize<xReal>::Locate(MKL::Minimizing::Inside, F, x, f);
            const xReal f_opt = F(x_opt);

            std::cerr << "ff=" << real_t(f_opt) << " @" << real_t(x_opt) << std::endl;

        }

        void Solver:: buildODE(XMLog &xml, const xReal f0)
        {
            Y_XML_SECTION(xml, "ODE");

            // accumulate
            for(size_t i=Cadd.size();i>0;--i)
                Cadd[i].free();

            for(const ProNode *pn=my.head;pn;pn=pn->next)
            {
                const Prospect   &pro = **pn;
                const xReal       pxi  = pro.xi;
                const xReal       rxi  = -pxi;
                const Components &cm   = pro.eq;
                for(const Actor *a=cm.prod->head;a;a=a->next) a->sp(Cadd,SubLevel) << (a->xn*pxi);
                for(const Actor *a=cm.reac->head;a;a=a->next) a->sp(Cadd,SubLevel) << (a->xn*rxi);
            }
            
            step.ld(zero);
            for(const SNode *sn=mix->species.head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                sp(step,SubLevel) = sp(Cadd,SubLevel).sum();
                if(xml.verbose)
                {
                    mix->sformat.print(xml() << "d[", sp, Justify::Right) << "] = " << real_t(sp(step,SubLevel)) << std::endl;
                }
            }

            makeStep(xml,f0);

        }


    }

}

