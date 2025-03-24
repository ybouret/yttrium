
#include "y/chemical/plexus/reactor.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        using namespace MKL;

        const char * const Reactor:: GenerateNR = "GenerateNR";

        xreal_t Reactor:: generateNR(XMLog &xml, const xreal_t S0, const XReadable & K0)
        {

            //__________________________________________________________________
            //
            //
            // get resources
            //
            //__________________________________________________________________
            const size_t      n = basis.size;
            Y_XML_SECTION_OPT(xml, GenerateNR, "|basis|=" << n);

            if(n<=0) {
                Y_XML_COMMENT(xml, "empty basis");
                return S0;
            }

            XArray  &    xi  = xiArr[n];   assert(n==xi.size());
            XMatrix &    Phi = phiArr[n];  assert(n==Phi.rows); assert(cluster.M==Phi.cols);
            XMatrix &    J   = jacArr[n];  assert(n==J.rows);   assert(n==J.cols);

            Phi.ld(0);
            
            //__________________________________________________________________
            //
            //
            // get jacobian and opposite of affinities
            //
            //__________________________________________________________________
            {
                size_t  i=1;
                for(const ENode *en=basis.head;en;en=en->next,++i)
                {
                    const Equilibrium &eq = **en;
                    xi[i] = -eq.jacobian(Phi[i], eq(K0,TopLevel), x_score, Cini, SubLevel);
                }
            }

            //__________________________________________________________________
            //
            //
            // J = Phi * Nu'
            //
            //__________________________________________________________________
            for(size_t i=n;i>0;--i)
            {
                const XReadable &phi = Phi[i]; assert(cluster.M==phi.size());
                XWritable       &jac = J[i];
                {
                    size_t j=1;
                    for(const ENode *en=basis.head;en;en=en->next,++j)
                    {
                        const Equilibrium  &eq = **en;
                        const XReadable    &nu = eq(cluster.xNu,SubLevel);
                        jac[j] = x_score.dot(phi,nu);
                    }
                }
            }

            //__________________________________________________________________
            //
            //
            // evaluate J^(-1)
            //
            //__________________________________________________________________
            if( !lu.build(J) )
            {
                Y_XML_COMMENT(xml, "singular system composition");
                return S0;
            }

            //__________________________________________________________________
            //
            //
            // evaluate xi
            //
            //__________________________________________________________________
            lu.solve(J,xi);
            
            //__________________________________________________________________
            //
            //
            // compute rates = Nu'*xi
            //
            //__________________________________________________________________
            rate.forEach( & XAdd::free );
            {
                size_t i=1;
                for(const ENode *en=basis.head;en;en=en->next,++i)
                    increaseRates(xi[i],**en);
            }
            
            return optimizedC(xml, S0, 2.0, "nr");
        }

    }

}


