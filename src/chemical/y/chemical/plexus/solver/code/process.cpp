
#include "y/chemical/plexus/solver.hpp"
#include "y/sort/heap.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Solver:: upgrade(XWritable &C, const Level L, const XReadable &Ktop, XMLog &xml)
        {
            Y_XML_SECTION(xml, "upgrade");

            size_t cycle = 0;

            //------------------------------------------------------------------
            //
            //
            // looking for running/crucial solutions
            //
            //
            //------------------------------------------------------------------
            ortho.free();
            basis.free();
        PROSPECT:
            {
                ++cycle;
                Y_XMLOG(xml, "[cycle #" << cycle << "]");
                pps.free();
                bool emergency = false; // keep only crucial otherwise

                //--------------------------------------------------------------
                //
                // loop over eqs
                //
                //------------------------------------------------------------------
                for(const ENode *en=mine.head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    const xreal_t      ek = Ktop[eq.indx[TopLevel]];
                    const size_t       ii = pps.size() + 1;
                    XWritable         &cc = mine.transfer(ceq[ii],SubLevel,C,L);
                    const Situation    st = afm.seek(cc, SubLevel, eq, ek);

                    switch(st)
                    {
                        case Blocked:
                            continue;

                        case Running:
                            if(emergency) continue;
                            Y_XMLOG(xml, "[Running] " << eq);
                            break;

                        case Crucial:
                            if(!emergency)
                            {
                                emergency = true;
                            }
                            Y_XMLOG(xml, "[Crucial] " << eq);
                            break;
                    }

                    const Prospect pro(st,eq,ek,cc,afm.eval(cc,SubLevel, C, L, eq) );
                    pps << pro;
                }

                //--------------------------------------------------------------
                //
                // special case: all blocks
                //
                //--------------------------------------------------------------
                if( pps.size() <= 0 )
                {
                    Y_XMLOG(xml, "[Jammed]");
                    return;
                }

                //--------------------------------------------------------------
                //
                // in case of emergency: move and try again
                //
                //--------------------------------------------------------------
                if(emergency)
                {
                    assert(pps.size()>0);

                    // remove not crucial
                    for(size_t i=pps.size();i>0;--i) {
                        if( Crucial != pps[i].st ) {
                            assert(Running==pps[i].st);
                            pps.remove(i);
                        }
                    }

                    Y_XML_SECTION_OPT(xml, "emergency","count='"<<pps.size() << "'");
                    assert( pps.size() > 0);

                    // sort remaingin crucial
                    HeapSort::Call(pps,Prospect::Compare);
                    showProspects(xml,Ktop);

                    const Prospect &pro = pps.head();
                    mine.transfer(C, L, pro.cc, SubLevel);
                    goto PROSPECT; // until no crucial was found

                }

            }

            assert(pps.size()>0);

            //------------------------------------------------------------------
            //
            //
            // looking for running/crucial solutions
            //
            //
            //------------------------------------------------------------------
            Y_XML_SECTION_OPT(xml, "running", "count='" << pps.size() << "'");
            HeapSort::Call(pps,Prospect::Compare);
            showProspects(xml,Ktop);


            //------------------------------------------------------------------
            //
            //
            // select family/basis of running equilibria
            //
            //
            //------------------------------------------------------------------
            {
                size_t i=1;
                for(;i<=pps.size();++i)
                {
                    const Prospect    &   pro = pps[i]; assert(Running==pro.st);
                    const Equilibrium &   eq  = pro.eq;
                    const size_t          ei  = eq.indx[SubLevel];
                    const Readable<int> & nu  = mine.iTopo[ei];

                    eq.mustSupport(C,L);
                    if(ortho.wouldAccept(nu))
                    {
                        ortho.expand();
                        basis << pro;
                        if(ortho.size>=dof) break;
                    }
                }

                for(++i;i<=pps.size();++i)
                {
                    const Prospect    &   pro = pps[i]; assert(Running==pro.st);
                    const Equilibrium &   eq  = pro.eq;
                    eq.mustSupport(C,L);
                }

            }

            if(xml.verbose)
            {
                xml() << "<family size='" << basis.size << "' dof='" << dof << "'>" << std::endl;
                for(const PNode *pn=basis.head;pn;pn=pn->next)
                {
                    xml() << "\t(+) " << (**pn).eq << std::endl;
                }
                xml() << "<family/>" << std::endl;
            }
        }

        void Solver:: process(XWritable &       C,
                              const Level       L,
                              const XReadable & Ktop,
                              XMLog           & xml)
        {

            Y_XML_SECTION(xml, "process");
            upgrade(C, L, Ktop, xml);


            {
                Y_XML_SECTION(xml, "evolve");
                //------------------------------------------------------------------
                //
                //
                // evolve
                //
                //
                //------------------------------------------------------------------
                switch( pps.size() )
                {
                    case 0:
                        throw Specific::Exception("here", "not possible");

                    case 1: {
                        const Prospect &pro = pps.head();
                        mine.transfer(C, L, pro.cc, SubLevel);
                    } return;

                    default:
                        break;
                }

                nrStage(C, L, xml);
                odeStep(C, L, Ktop, xml);
            }
        }



    }

}


