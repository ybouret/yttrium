
#include "y/chemical/plexus/reactor.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        using namespace MKL;


        xreal_t Reactor:: generateNR(XMLog &xml, const xreal_t S0, const XReadable & K0)
        {
            static const char fn[] = "GenerateNR";
            const size_t      n = basis.size;
            Y_XML_SECTION_OPT(xml, fn, "|basis|=" << n);

            if(n<=0)
            {
                Y_XML_COMMENT(xml, "empty basis");
                return S0;
            }

            const size_t m = cluster->species->size;
            XMatrix Phi(n,m);
            XArray  xi(n);
            Phi.ld(0);
            xi.ld(0);

            {
                size_t  i=1;
                for(const ENode *en=basis.head;en;en=en->next,++i)
                {
                    const Equilibrium &eq = **en;
                    xi[i] = -eq.jacobian(Phi[i], eq(K0,TopLevel), x_score, Cini, SubLevel);
                }
            }
            std::cerr << "Phi =  " << Phi << std::endl;
            std::cerr << "A   = -" << xi << std::endl;

            XMatrix J(n,n);
            for(size_t i=n;i>0;--i)
            {
                const XReadable &phi = Phi[i]; assert(m==phi.size());
                XWritable       &jac = J[i];
                {
                    size_t j=1;
                    for(const ENode *en=basis.head;en;en=en->next,++j)
                    {
                        const Equilibrium   &eq = **en;
                        const Readable<int> &nu = eq(cluster.Nu,SubLevel);
                        jac[j] = x_score.dot(phi,nu);
                    }
                }
            }
            std::cerr << "J=" << J << std::endl;

            LU<xreal_t> lu(n);

            if( !lu.build(J) )
            {
                Y_XML_COMMENT(xml, "singular system");
                return S0;
            }

            lu.solve(J,xi);
            std::cerr << "xi=" << xi << std::endl;



            return S0;

        }

    }

}


