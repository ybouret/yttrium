
#include "y/chemical/plexus/reactor.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        xreal_t Reactor:: queryRates(XMLog &xml, const xreal_t S0)
        {
            static const char fn[] = "QueryRates";
            Y_XML_SECTION(xml,fn);

            // accumulate virtual rates
            rate.forEach( & XAdd::free );
            for(const OutNode *node=running.head;node;node=node->next)
            {
                const Outcome       &out = **node; if(out.ax.mantissa<=0) continue;
                const Components    &eq  = out.eq;
                const xreal_t       pxi  = out.xi;
                const xreal_t       rxi  = -pxi;
                for(const Actor *a=eq.reac->head;a;a=a->next) a->sp(rate,SubLevel).insert(rxi,a->nu);
                for(const Actor *a=eq.prod->head;a;a=a->next) a->sp(rate,SubLevel).insert(pxi,a->nu);
            }

            // construct increment
            const xreal_t zero = 0;
            xreal_t       rho  = 10;
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

            if(cut) rho *= 0.99;

            {
            EVAL_Cend:
                for(size_t i=Cend.size();i>0;--i)
                {
                    if( (Cend[i] = Cini[i] + rho * dC[i]) < zero )
                    {
                        rho *= 0.99;
                        goto EVAL_Cend;
                    }
                }
            }

            if(Trace) {
                const String baseName = "rate";
                saveCurrentProfile(baseName,1000);
                tracing << baseName;
            }

            // locate guess
            const xreal_t Sr = optimize1D(S0);
            Y_XML_COMMENT(xml, fn << " result");
            Y_XMLOG(xml, "Sr=" << Sr.str() << " // S0=" << S0.str() );

            return Sr;
        }
    }

}


