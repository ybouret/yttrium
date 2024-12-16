#include "y/chemical/plexus/solver.hpp"
#include "y/mkl/opt/minimize.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        xReal Solver:: makeStep(XMLog &            xml,
                                const xReal        f0,
                                const char * const sid)
        {
            assert(0!=sid);
            Y_XML_SECTION_OPT(xml, "makeStep", "name='" << sid << "'");

            //------------------------------------------------------------------
            //
            // study step w.r.t Cini
            //
            //------------------------------------------------------------------
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
            Y_XMLOG(xml, sid << ".hasCut=" << hasCut);
            Y_XMLOG(xml, sid << ".factor=" << real_t(factor) );


            if(hasCut)
            {
                factor *= 0.95;
            }

            //------------------------------------------------------------------
            //
            // build Cend
            //
            //------------------------------------------------------------------
            for(const SNode *sn=mix->species.head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                sp(Cend,SubLevel) = sp(Cini,SubLevel) + factor * sp(step,SubLevel);
                assert(sp(Cend,SubLevel) >= 0.0 );
            }
            
            {
                String     fn = sid; fn += ".dat";
                save(fn,100);
            }

            //------------------------------------------------------------------
            //
            // optimize
            //
            //------------------------------------------------------------------
            const xReal f1    = objectiveFunction(Cend,SubLevel);
            const xReal f_opt = optimize(f0,f1);
            Y_XMLOG(xml, sid << ".ff=" << std::setw(15) << real_t(f1) << " => " << std::setw(15) << real_t(f_opt));

            return f_opt;
        }
        
    }

}


