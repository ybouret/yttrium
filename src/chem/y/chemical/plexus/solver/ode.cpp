#include "y/chemical/plexus/solver.hpp"


namespace Yttrium
{
    namespace Chemical
    {


        void Solver:: buildODE(XMLog &xml, const xReal f0)
        {
            Y_XML_SECTION(xml, "ODE");

            //------------------------------------------------------------------
            //
            // accumulate virtual rates
            //
            //------------------------------------------------------------------
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

            //------------------------------------------------------------------
            //
            // compose step
            //
            //------------------------------------------------------------------
            step.ld(zero);
            for(const SNode *sn=mix->species.head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                sp(step,SubLevel) = sp(Cadd,SubLevel).sum();
                if(xml.verbose)
                {
                    mix->sformat.print(xml() << "d[", sp, Justify::Right) << "] = " << std::setw(15) << real_t(sp(step,SubLevel)) << std::endl;
                }
            }


            //------------------------------------------------------------------
            //
            // make step
            //
            //------------------------------------------------------------------
            makeStep(xml,f0,"ode");

        }


    }

}

