#include "y/chemical/plexus/solver.hpp"
#include "y/chemical/plexus/mix.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        
        xReal Solver:: computeStepDS(XMLog &xml, const xReal f0)
        {
            static const char fn[] = "DynamicalSystem";
            Y_XML_SECTION(xml,fn);
            assert(isAcceptable(Cini,SubLevel));

            //------------------------------------------------------------------
            //
            //
            // accumulate virtual rates
            //
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
            //
            // compose step and check scaling
            //
            //
            //------------------------------------------------------------------
            step.ld(zero);
            bool  mustCut = false;
            xReal scaling =     2; // default scaling

            for(const SNode *sn=mix->species.head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                const xReal    dc = sp(step,SubLevel) = sp(Cadd,SubLevel).sum();
                const xReal    cc = sp(Cini,SubLevel); assert(cc>=0.0);


                if(dc<0.0)
                {
                    const xReal smax = cc/(-dc);
                    if(smax<scaling)
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
            // prepare Cend from step
            //
            //
            //------------------------------------------------------------------
            if(mustCut) scaling *= safe;
            setRecentStep(scaling);
            while( !isAcceptable(Cend, SubLevel) )
            {
                scaling *= safe;
                setRecentStep(scaling);
            }
            save("ode.dat",100);

            //------------------------------------------------------------------
            //
            //
            // look for optimized position
            //
            //
            //------------------------------------------------------------------
            const xReal f1   = optimize(f0, objectiveFunction(Cend,SubLevel) );
            Y_XML_COMMENT(xml, fn << " scaling was " << real_t(scaling) );
            Y_XML_COMMENT(xml, fn << " function is " << real_t(f1) << " / " << real_t(f0) );

            return f1;
        }

    }

}
