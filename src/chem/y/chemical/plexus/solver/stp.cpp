
#include "y/chemical/plexus/solver.hpp"
#include "y/stream/libc/output.hpp"
#include "y/mkl/opt/minimize.hpp"

#include "y/stream/libc/output.hpp"

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
            Y_XMLOG(xml, "hasCut=" << hasCut);
            Y_XMLOG(xml, "factor=" << real_t(factor) );


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
            XTriplet x = { zero, zero, one };
            XTriplet f = { f0,   f0,   objectiveFunction(Cend,SubLevel) };
            
            std::cerr << "ff=" << real_t(f.c) << std::endl;
            Solver &    F     = *this;
            const xReal x_opt = MKL::Minimize<xReal>::Locate(MKL::Minimizing::Inside, F, x, f);
            const xReal f_opt = F(x_opt);
            
            std::cerr << "ff=" << real_t(f_opt) << " @" << real_t(x_opt) << std::endl;
            
            
            return f_opt;
        }
        
    }

}


