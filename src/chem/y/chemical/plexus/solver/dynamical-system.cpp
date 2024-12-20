
#include "y/chemical/plexus/solver.hpp"
#include "y/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        void Solver:: computeStepDS(XMLog &xml)
        {
            Y_XML_SECTION(xml, "DynamicalSystem.Step");

            //------------------------------------------------------------------
            //
            // accumulate virtual rates
            //
            //------------------------------------------------------------------
            Cadd.forEach( & XAdd::free );

            for(const ProNode *pn=my.head;pn;pn=pn->next)
            {
                const Prospect   &pro = **pn;
                const xReal       pxi  = pro.xi;
                const xReal       rxi  = -pxi;
                const Components &cm   = pro.eq;
                for(const Actor *a=cm.prod->head;a;a=a->next) a->sp(Cadd,SubLevel) << (a->xn*pxi);
                for(const Actor *a=cm.reac->head;a;a=a->next) a->sp(Cadd,SubLevel) << (a->xn*rxi);
            }

            //------------------------------------------------------------------
            //
            // compose step
            //
            //------------------------------------------------------------------
            step.ld(zero);
            bool  mustCut = false;
            xReal scaling = 0;

            for(const SNode *sn=mix->species.head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                const xReal    dc = sp(step,SubLevel) = sp(Cadd,SubLevel).sum();
                const xReal    cc = sp(Cini,SubLevel); assert(cc>0.0);


                if(dc<0.0)
                {
                    const xReal smax = cc/(-dc);
                    if(mustCut)
                    {
                        scaling = Min(scaling,smax);
                    }
                    else
                    {
                        mustCut = true;
                        scaling = smax;
                    }
                }


                if(xml.verbose)
                {
                    mix->sformat.print(xml() << "d[", sp, Justify::Right)
                    << "] = " << std::setw(15) << real_t(dc)
                    << " / "  << std::setw(15) << real_t(cc)
                    << " => scaling=" << std::setw(15) << real_t(scaling)
                    << std::endl;
                }


            }


            //------------------------------------------------------------------
            //
            //
            //
            //------------------------------------------------------------------



            throw Exception("Not Implemented Yet");
        }

    }

}
