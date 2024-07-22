
#include "y/chemical/plexus/solver.hpp"
#include "y/sort/heap.hpp"
#include "y/mkl/tao/seq/level1.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        Solver:: ~Solver() noexcept
        {
        }

        Solver:: Solver(const Clusters &cls) :
        Ceq(cls.maxEPC,cls.maxSPC),
        dCe(cls.maxEPC,cls.maxSPC),
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
                Y_XML_SECTION(xml, "Probe");
                //--------------------------------------------------------------
                //
                // Initialize prospects
                //
                //--------------------------------------------------------------
                pps.free();

                //--------------------------------------------------------------
                //
                // Store all prospects matching active equilibria
                //
                //--------------------------------------------------------------
                for(const ENode *en=cl.head;en;en=en->next)
                {
                    const Equilibrium    &eq   = **en;
                    const size_t * const  in   = eq.indx;
                    const xreal_t         eK   = K[in[TopLevel]];
                    const size_t          isub = in[SubLevel];
                    XWritable            &Ci   = Ceq[isub];
                    XWritable            &Di   = dCe[isub];
                    XWritable            &phi  = Phi[isub];
                    if( afm.solve(Ci, SubLevel, C, TopLevel, eq, eK) )
                    {
                        //const xreal_t        xi = afm.eval(Ci, SubLevel, C, TopLevel, eq);
                        const xreal_t  xi = afm.eval(Di,Ci,SubLevel, C, TopLevel, eq);
                        const Prospect pro(eq,eK,xi,Ci,Di,phi);
                        pps << pro;
                    }
                }

                if(pps.size()<=0)
                {
                    Y_XMLOG(xml, "[[no active equilibrium]]");
                    return;
                }

                //--------------------------------------------------------------
                //
                // order them in decreasing |xi|
                //
                //--------------------------------------------------------------
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

            LinearlyIndependent &li = lis[cl.species.size];
            {
                Y_XML_SECTION(xml, "Basis");

                //--------------------------------------------------------------
                //
                // find basis
                //
                //--------------------------------------------------------------
                const size_t         np = pps.size();
                const size_t         nm = cl.Nu.rows;
                li.init();
                for(size_t i=1;i<=np;++i)
                {
                    Prospect &pro = pps[i];
                    if(li.keep(pro,cl.topology) && li->size >= nm) break;
                }

                //--------------------------------------------------------------
                //
                // update prospect(s) in basis
                //
                //--------------------------------------------------------------
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


            {
                Y_XML_SECTION(xml, "Global");
                PNode           * node  = li->head;
                Prospect        & lead  = **node;
                const XReadable & dC    = lead.dc;
                if(xml.verbose)
                {
                    cl.uuid.pad(xml() << "lead: " << lead.eq.name,lead.eq);
                    *xml <<        " @" << std::setw(15) << real_t(lead.xi);
                    *xml << std::endl;
                }

                afm.xadd.make(cl.species.size);
                for(node=node->next;node;node=node->next)
                {
                    Prospect     &pro = **node;
                    const xreal_t dot = MKL::Tao::DotProduct<xreal_t>::Of_(dC, pro.dc, afm.xadd);
                    if(xml.verbose)
                    {
                        cl.uuid.pad(xml() << "pro : " << pro.eq.name,pro.eq);
                        *xml <<        " dot = " << std::setw(15) << real_t(dot);
                        *xml << std::endl;
                    }
                }
            }


        }


    }


}
