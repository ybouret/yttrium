
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

            {
                Y_XML_SECTION(xml, "Scan");
                // initialize prospects
                pps.free();

                // store all prospects
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
                        const Prospect       pro(eq,eK,xi,Ci,phi);
                        pps << pro;
                    }
                }

                // order them in decreasing |xi|
                HeapSort::Call(pps, Prospect::Compare);
                if(xml.verbose)
                {
                    for(size_t i=1;i<=pps.size();++i)
                    {
                        const Prospect &pro = pps[i];
                        cl.uuid.pad(xml() << pro.eq.name,pro.eq) << " @" << std::setw(15) << real_t(pro.xi) << std::endl;
                    }
                }
            }

            {
                Y_XML_SECTION(xml, "Base");


                // find sub basis
                LinearlyIndependent &li = lis[cl.species.size];
                const size_t         np = pps.size();
                const size_t         nm = cl.Nu.rows;
                li.init();
                for(size_t i=1;i<=np;++i)
                {
                    Prospect &pro = pps[i];
                    if(li.keep(pro,cl.topology) && li->size >= nm) break;
                }


                // update prospect
                for(PNode *pn=li->head;pn;pn=pn->next)
                {
                    Prospect &pro = **pn;
                    pro.update(afm.xadd, afm.xmul);
                    if(xml.verbose)
                    {
                        cl.uuid.pad(xml() << pro.eq.name,pro.eq);
                        *xml <<        " @" << std::setw(15) << real_t(pro.xi);
                        *xml << " : slope=" << std::setw(15) << real_t(pro.sl) << std::endl;
                    }
                }
            }


        }


    }


}
