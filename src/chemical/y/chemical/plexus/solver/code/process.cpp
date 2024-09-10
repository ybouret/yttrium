
#include "y/chemical/plexus/solver.hpp"
#include "y/sort/heap.hpp"
#include "y/system/exception.hpp"
#include "y/mkl/opt/minimize.hpp"
#include "y/mkl/opt/bracket.hpp"

#include "y/jive/pattern/vfs.hpp"
#include "y/vfs/local-fs.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        using namespace MKL;

        void Solver:: upgrade(XWritable &C, const Level L, const XReadable &Ktop, XMLog &xml)
        {
            Y_XML_SECTION(xml, "upgrade");

            Jive::VirtualFileSystem::TryRemove(LocalFS::Instance(), ".", "pro", VFS::Entry::Ext);

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
                    XWritable         &dc = deq[ii];
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

                    const Prospect pro(st,eq,ek,cc,afm.eval(dc,cc,SubLevel,C,L,eq),dc);
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
            // We now have Running only solutions : objFunc is available
            // -> we can compute objFunc(pro) and order them
            //
            //------------------------------------------------------------------
            {
                Y_XML_SECTION_OPT(xml,"running", "count='" << pps.size() << "'");
                for(size_t i=pps.size();i>0;--i)
                {
                    Prospect &pro = pps[i]; assert(Running==pro.st);
                    pro.ff = objFunc(pro.cc,SubLevel);
                }
                HeapSort::Call(pps,Prospect::Compare);
                showProspects(xml,Ktop);
            }

            mine.transfer(Cin, SubLevel, C, L);
            const xreal_t A0 = objGrad(Cin,SubLevel);
            //MKL::Bracketing::Verbose = true;
            if(pps.size()>1)
            {
                Y_XML_SECTION(xml,"xselect");

                //--------------------------------------------------------------
                //
                // set common starting points
                //
                //--------------------------------------------------------------

                Solver       &F  = *this;
                Y_XMLOG(xml, "|               |" << Formatted::Get("%15.4f", real_t(A0)) << "| = A0");

                //--------------------------------------------------------------
                //
                // optimize each
                //
                //--------------------------------------------------------------
                for(size_t i=1;i<=pps.size();++i)
                {
                    Prospect &    pro = pps[i];
                    const xreal_t sig = afm.xadd.dot(pro.dc,grd); // slope

                    if(sig.mantissa>=0.0)
                    {
                        //------------------------------------------------------
                        // cancel this position
                        //------------------------------------------------------
                        saveProfile(pro);
                        mine.transfer(pro.cc, SubLevel, C, L);
                        pro.dc.ld(pro.xi=0);
                        pro.ff = A0;
                    }
                    else
                    {
                        //------------------------------------------------------
                        // initialize end point and triplets
                        //------------------------------------------------------
                        Cex.ld(pro.cc);
                        Triplet<xreal_t> uu   = { 0,  -1, 1      };
                        Triplet<xreal_t> ff   = { A0, -1, pro.ff };

                        //------------------------------------------------------
                        // apply minimize
                        //------------------------------------------------------
                        const xreal_t    uopt = Minimize<xreal_t>::Locate(Minimizing::Inside, F, uu, ff);
                        const xreal_t    Fopt = F(uopt);

                        //------------------------------------------------------
                        // update status: cc and xi
                        //------------------------------------------------------
                        pro.ff = Fopt;
                        pro.cc.ld(Cws);
                        pro.xi = afm.eval(pro.dc, pro.cc, SubLevel, Cin, SubLevel, pro.eq);
                        saveProfile(pro);

                    }

                    //if(xml.verbose) pro.show( xml(), mine, &Ktop) << " | slope = " << real_t(sig) << std::endl;

                }

                //--------------------------------------------------------------
                //
                // get new order
                //
                //--------------------------------------------------------------
                HeapSort::Call(pps,Prospect::Compare);
                showProspects(xml,Ktop);
            }





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
                Y_XML_SECTION_OPT(xml, "family",  "size='" << basis.size << "' dof='" << dof << "'");
                //xml() << "|" << std::setw(15) << "A0=" << "|" << Formatted::Get("%15.4f", real_t( objFunc(C, L) ) ) << "|" << std::endl;
                Y_XMLOG(xml, "|               |" << Formatted::Get("%15.4f", real_t(A0)) << "| = A0");
                for(const PNode *pn=basis.head;pn;pn=pn->next)
                {
                    const Prospect &pro = **pn;
                    //const xreal_t   aff = objFunc(pro.cc, SubLevel);
                    pro.show(xml(), mine, 0) << std::endl;
                }
            }

            throw Exception("Not Finished");

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

                return;
                odeStep(C, L, Ktop, xml);
            }
        }



    }

}


