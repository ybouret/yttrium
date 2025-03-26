

#include "y/chemical/plexus/reactor.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        xreal_t Reactor:: optimizedC(XMLog &            xml,
                                     const xreal_t      S0,
                                     xreal_t            rho,
                                     const char * const uid)
        {
            assert(0!=uid);
            assert(rho>0.0);
            Y_XML_SECTION_OPT(xml, "OptimizedC", "uid=" << uid);

            //------------------------------------------------------------------
            //
            //
            // start from scaling=rho
            //
            //
            //------------------------------------------------------------------
            const xreal_t zero = 0;
            const xreal_t safe = 0.99;
            bool          cut  = false;
            for(const SNode *sn=cluster->species->head;sn;sn=sn->next)
            {
                const Species &sp  = **sn;
                const xreal_t  c0  = sp(Cini,SubLevel); assert(c0>=0.0);
                xreal_t &      dc  = sp(dC,  SubLevel) = sp(rate,SubLevel).sum();

                if(dc<zero) {
                    const xreal_t fac = (-c0)/dc;
                    if(fac<=rho)
                    {
                        cut = true;
                        rho = fac;
                    }
                }
                if(xml.verbose)
                {
                    cluster->sformat.pad( xml() << "[" << sp.name << "]",sp)
                    << " = "  << std::setw(24) << c0.str()
                    << " + (" << std::setw(24) << dc.str() << ") * " << rho.str()
                    << std::endl;
                }
            }

            //------------------------------------------------------------------
            //
            //
            // Evaluate Cend
            //
            //
            //------------------------------------------------------------------
            if(cut) rho *= safe; // no zero active species...

            {
            EVAL_Cend:
                for(size_t i=Cend.size();i>0;--i)
                {
                    if( (Cend[i] = Cini[i] + rho * dC[i]) < zero )
                    {
                        rho *= safe;
                        goto EVAL_Cend;
                    }
                }
            }

            if(EmitProfiles) {
                const String baseName = uid;
                saveCurrentProfile(baseName,100);
                profiles << baseName;
            }

            //------------------------------------------------------------------
            //
            //
            // locate guess
            //
            //
            //------------------------------------------------------------------
            const xreal_t Stry = optimize1D(S0);
            Y_XML_COMMENT(xml, uid << " result");
            Y_XMLOG(xml, "S" << uid << "=" << Stry.str() << " // S0=" << S0.str() );
            return Stry;
        }


     

    }

}


