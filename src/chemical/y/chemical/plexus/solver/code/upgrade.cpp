

#include "y/chemical/plexus/solver.hpp"
#include "y/sort/heap.hpp"
#include "y/system/exception.hpp"
#include "y/mkl/opt/minimize.hpp"
#include "y/mkl/opt/bracket.hpp"

#include "y/jive/pattern/vfs.hpp"
#include "y/vfs/local/fs.hpp"
#include "y/stream/libc/output.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        using namespace MKL;


        size_t Solver:: upgrade(XWritable &C, const Level L, const XReadable &Ktop, XMLog &xml)
        {
            Y_XML_SECTION(xml, "upgrade");

            Jive::VirtualFileSystem::TryRemove(LocalFS::Instance(), ".", "pro", VFS::Entry::Ext);

            //------------------------------------------------------------------
            //
            //
            // initialize
            //
            //
            //------------------------------------------------------------------
            ff0 = 0;
            vlist.free();
            ortho.free();
            basis.free();

            //------------------------------------------------------------------
            //
            //
            // looking for running/crucial solutions
            //
            //
            //------------------------------------------------------------------
            size_t cycle = 0;
        PROSPECT:
            {
                bool emergency = false; // start from no-crucial state
                Y_XML_COMMENT(xml, "cycle #" << ++cycle);
                pps.free();

                //--------------------------------------------------------------
                //
                // loop over equilibria and collect prospects
                //
                //---------------------------------------------------------------
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
                        case Blocked: continue;

                        case Running: if(emergency) continue; // on crucial state
                            Y_XMLOG(xml, "(+) " << eq);
                            break;

                        case Crucial: emergency = true;
                            Y_XMLOG(xml, "(!) " << eq);
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
                if( pps.size() <= 0 ) {
                    Y_XML_COMMENT(xml,"Jammed");
                    return 0;
                }

                //--------------------------------------------------------------
                //
                // in case of emergency: find right move and try again
                //
                //--------------------------------------------------------------
                if(emergency)
                {
                    assert(pps.size()>0);

                    //----------------------------------------------------------
                    // remove not crucial
                    //----------------------------------------------------------
                    for(size_t i=pps.size();i>0;--i) {
                        if( Crucial != pps[i].st ) {
                            assert(Running==pps[i].st);
                            pps.remove(i);
                        }
                    }

                    Y_XML_SECTION_OPT(xml, "emergency","count='"<<pps.size() << "'");
                    assert( pps.size() > 0);

                    //----------------------------------------------------------
                    // sort remaining crucial by decreasing |xi|
                    //----------------------------------------------------------
                    HeapSort::Call(pps,Prospect::CompareDecreasingAX);
                    showProspects(xml,Ktop);

                    //----------------------------------------------------------
                    // take greatest |xi| to avoid smallest concentrations
                    //----------------------------------------------------------
                    const Prospect &pro = pps.head();
                    mine.transfer(C, L, pro.cc, SubLevel);
                    goto PROSPECT; // until no crucial was found
                }

            }

            const size_t npro = pps.size();
            assert(npro>0);
            assert(0==ortho.size);
            assert(0==basis.size);

            //------------------------------------------------------------------
            //
            //
            // We now have Running only solutions : objFunc is available
            //
            //
            //------------------------------------------------------------------
            for(size_t i=npro;i>0;--i)
            {
                Prospect &pro = pps[i]; 
                assert(Running==pro.st);
                pro.ff = objFunc(pro.cc,SubLevel);
            }




            //------------------------------------------------------------------
            //
            //
            // set common starting point : ff0@Cin, and study each prospect
            //
            //
            //------------------------------------------------------------------
            ff0 = objGrad(mine.transfer(Cin,SubLevel,C,L),SubLevel);

            if(1==npro)
            {
                assert(0==basis.size);
                const Prospect &pro = pps[1];
                basis << pro;
                return 1;
            }

            assert(npro>=2);

            //------------------------------------------------------------------
            //
            //
            // select promising solutions
            //
            //
            //------------------------------------------------------------------
            {
                Y_XML_SECTION_OPT(xml,"running", "count='" << pps.size() << "'");
                Y_XMLOG(xml, "|               |" << Formatted::Get("%15.4g", real_t(ff0)) << "| = ff0");

                //--------------------------------------------------------------
                //
                // optimize each
                //
                //--------------------------------------------------------------
                for(size_t i=npro;i>0;--i)
                {
                    Prospect &    pro = pps[i];
                    const xreal_t sig = afm.xadd.dot(pro.dc,grd); // slope

                    if(sig.mantissa>=0.0)
                    {
                        //------------------------------------------------------
                        // positive or zero slope, cancel this position
                        //------------------------------------------------------
                        mine.transfer(pro.cc, SubLevel, C, L);
                        pro.dc.ld(pro.ax=pro.xi=0);
                        pro.ff = ff0;
                    }
                    else
                    {
                        //------------------------------------------------------
                        // initialize end point and triplets
                        //------------------------------------------------------
                        Cex.ld(pro.cc);
                        Triplet<xreal_t> uu   = { 0,   -1, 1      };
                        Triplet<xreal_t> ff   = { ff0, -1, pro.ff };

                        //------------------------------------------------------
                        // update status: cc and xi
                        //------------------------------------------------------
                        pro.ff = fcn( Minimize<xreal_t>::Locate(Minimizing::Inside, fcn, uu, ff) );
                        pro.cc.ld(Cws);
                        pro.ax = (pro.xi = afm.eval(pro.dc, pro.cc, SubLevel, Cin, SubLevel, pro.eq)).abs();
                    }
                }

                //--------------------------------------------------------------
                //
                // get new order to build basis
                //
                //--------------------------------------------------------------
                HeapSort::Call(pps,Prospect::CompareIncreasingFF);
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
                Y_XMLOG(xml, "|               |" << Formatted::Get("%15.4g", real_t(ff0)) << "| = ff0");
                for(const PNode *pn=basis.head;pn;pn=pn->next)
                {
                    (**pn).show(xml(), mine, 0) << std::endl;
                }
            }

            return npro;
        }
    }

}

