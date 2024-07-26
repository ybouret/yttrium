#include "y/chemical/plexus/solver.hpp"
#include "y/type/temporary.hpp"
#include "y/sort/heap.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Solver:: ~Solver() noexcept {}


        xreal_t Solver:: objectiveFunction(const XReadable &C,
                                           const Level      L)
        {
            assert(0!=ppb);
            const PList &list = **ppb;
            {
                size_t indx = 0;
                for(const PNode *pn=list.head;pn;pn=pn->next)
                {
                    obj[++indx] = (**pn).objectiveFunction(C,L,afm.xmul);
                }
            }
            const LightArray<xreal_t> arr( &obj[1], list.size );
            return afm.xadd.normOf(arr);
        }


        void Solver:: run(const Cluster   &cl,
                          XWritable       &Ctop,
                          const XReadable &Ktop,
                          XMLog           &xml)
        {
            const xreal_t zero;
            const xreal_t mOne(-1);
            const size_t  ndof = cl.Nu.rows; assert(ndof>0); // primary equilibria

            {
                //--------------------------------------------------------------
                //
                //
                Y_XML_SECTION(xml, "Scanning");
                //
                //
                //--------------------------------------------------------------
            SCAN:
                Y_XMLOG(xml, "(*) [Examine]");
                obj.free();
                pps.free();
                for(const ENode *en=cl.head;en;en=en->next)
                {
                    //----------------------------------------------------------
                    //
                    // get equilibrium and its aftermath
                    //
                    //----------------------------------------------------------
                    const Equilibrium &eq = **en;
                    const xreal_t      eK = Ktop[ eq.indx[TopLevel] ];
                    const size_t       ii = obj.size() + 1;
                    XWritable         &cc = ceq[ii];
                    const Situation    id = afm.seek( cl.transfer(cc,SubLevel,Ctop,TopLevel), SubLevel, eq, eK);
                    switch(id)
                    {
                        case Blocked: Y_XMLOG(xml, "(-) [Blocked] " << eq); continue;
                        case Running: Y_XMLOG(xml, "(+) [Running] " << eq); break;
                        case Crucial:
                            Y_XMLOG(xml, "(!) [Crucial] " << eq << " @" << cc);
                            cl.transfer(Ctop, TopLevel, cc, SubLevel); // set crucial
                            goto SCAN;                                 // abort and restart
                    }
                    assert(Running==id);

                    //----------------------------------------------------------
                    //
                    // store new running prospect
                    //
                    //----------------------------------------------------------
                    eq.drvsMassAction(eK, phi, SubLevel, cc, SubLevel, afm.xmul);
                    const xreal_t  slope = eq.dot(phi, SubLevel, afm.xadd);
                    if(slope>=zero) throw Specific::Exception(eq.name.c_str(),"invalid slope");
                    const Prospect pro(eq, eK, cc, afm.eval(cc,SubLevel,Ctop,TopLevel,eq), mOne/slope );
                    pps << pro;
                    obj << pro.objectiveFunction(Ctop, TopLevel, afm.xmul);
                }

                if( pps.size() <= 0 ) {
                    Y_XMLOG(xml, "[Jammed!]");
                    return;
                }

                {
                    //----------------------------------------------------------
                    //
                    Y_XML_SECTION_OPT(xml, "Individual", " running='" << pps.size() << "' clusterSize='" << cl.size << "'" );
                    //
                    //----------------------------------------------------------
                    HeapSort::Call(obj, Comparison::Decreasing<xreal_t>, pps);
                    if(xml.verbose)
                    {
                        for(size_t i=1;i<=pps.size();++i)
                        {
                            const Prospect &pro = pps[i];
                            cl.uuid.pad(xml() << pro.eq,pro.eq);
                            *xml << " | ham=" << std::setw(15) << real_t(obj[i]);
                            *xml << " | ax =" << std::setw(15) << real_t(pro.xi.abs());
                            pro.eq.displayCompact(*xml << " @", pro.cc, SubLevel);
                            *xml << " ks=" << real_t(pro.ks);
                            *xml << std::endl;
                        }
                    }
                }
            }


            QBuilder &                 base = qdb[cl.species.size];
            const Temporary<QBuilder*> keep(ppb,&base);
            {
                //--------------------------------------------------------------
                //
                //
                Y_XML_SECTION(xml, "Extracting");
                //
                //
                //--------------------------------------------------------------
                {
                    //----------------------------------------------------------
                    //
                    Y_XML_SECTION(xml, "SubFamily");
                    //
                    //----------------------------------------------------------
                    const size_t  npmx = pps.size();
                    base.init();
                    for(size_t i=1;i<=npmx;++i)
                    {
                        const Prospect &pro = pps[i];
                        if(base.grow(pro, cl.topology) && base->size >= ndof) break;
                    }

                    Y_XMLOG(xml, "#dof = " << std::setw(10) << ndof);
                    Y_XMLOG(xml, "#pro = " << std::setw(10) << npmx);
                    Y_XMLOG(xml, "#vec = " << std::setw(10) << base->size);
                }

                {
                    //----------------------------------------------------------
                    //
                    Y_XML_SECTION(xml, "PhaseSpace");
                    //
                    //----------------------------------------------------------
                    for(const PNode *pn=base->head;pn;pn=pn->next)
                    {
                        const Prospect &pro = **pn;
                        const xreal_t   ham = objectiveFunction(pro.cc,SubLevel);
                        if(xml.verbose)
                        {
                            cl.uuid.pad(xml() << "F(" << pro.eq << ")",pro.eq) << " : " << std::setw(15) << real_t(ham);
                            *xml << std::endl;
                        }
                    }
                }

            }




        }
    }
}
