
#include "y/chemical/plexus/solver.hpp"
#include "y/sort/heap.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        Solver:: ~Solver() noexcept
        {
        }

        Solver:: Solver(const Clusters &cls) :
        Ceq(cls.maxEPC,cls.maxSPC),
        Phi(cls.maxEPC,cls.maxSPC),
        afm(),
        pps(cls.maxEPC),
        lis()
        {

            for(const Cluster *cl=cls->head;cl;cl=cl->next)
            {
                lis(cl->size,cl->species.size);
            }
        }

        void Solver:: process(XWritable       &C,
                              const Cluster   &cl,
                              const XReadable &K,
                              XMLog           &xml)
        {
            Y_XML_SECTION_OPT(xml, "Solver::Cluster", " eqs='" << cl.size << "'");
            pps.free();
            for(const ENode *en=cl.head;en;en=en->next)
            {
                const Equilibrium    &eq   = **en;
                const size_t * const  in   = eq.indx;
                const xreal_t         eK   = K[in[TopLevel]];
                const size_t          isub = in[SubLevel];
                XWritable            &Ci   = Ceq[isub];
                XWritable            &phi  = Phi[isub];
                if( afm.solve(Ci, SubLevel, C, TopLevel, eq, eK) )
                {
                    const xreal_t        xi = afm.eval(Ci, SubLevel, C, TopLevel, eq);
                    const Prospect       pro(eq,xi,Ci,phi);
                    pps << pro;
                }
            }

            HeapSort::Call(pps, Prospect::Compare);
            const size_t n = pps.size();
            for(size_t i=1;i<=n;++i)
            {
                const Prospect &pro = pps[i];
                if(xml.verbose)
                {
                    cl.uuid.pad(xml() << pro.eq.name,pro.eq) << " @" << std::setw(15) << real_t(pro.xi) << std::endl;
                }
            }

            

        }


    }


}
