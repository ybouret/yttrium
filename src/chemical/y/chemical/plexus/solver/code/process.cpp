
#include "y/chemical/plexus/solver.hpp"
#include "y/sort/heap.hpp"
#include "y/system/exception.hpp"
#include "y/mkl/opt/minimize.hpp"
#include "y/mkl/opt/bracket.hpp"

#include "y/jive/pattern/vfs.hpp"
#include "y/vfs/local-fs.hpp"
#include "y/stream/libc/output.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        using namespace MKL;


        void Solver:: upgrade(XWritable &C, const Level L, const XReadable &Ktop, XMLog &xml)
        {
            Y_XML_SECTION(xml, "upgrade");

            Jive::VirtualFileSystem::TryRemove(LocalFS::Instance(), ".", "pro", VFS::Entry::Ext);

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
                bool emergency = false; // keep only crucial otherwise
                Y_XMLOG(xml, "[cycle #" << ++cycle << "]");
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

                        case Running: if(emergency) continue;
                            Y_XMLOG(xml, "[Running] " << eq);
                            break;

                        case Crucial: emergency = true;
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
                if( pps.size() <= 0 ) {
                    Y_XMLOG(xml, "[Jammed]");
                    return;
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
            assert(pps.size()>0);
            assert(0==ortho.size);
            assert(0==basis.size);

            //------------------------------------------------------------------
            //
            //
            // We now have Running only solutions : objFunc is available
            // -> we can compute objFunc(pro) and pre-order them
            //
            //------------------------------------------------------------------
            {
                Y_XML_SECTION_OPT(xml,"running", "count='" << pps.size() << "'");
                for(size_t i=pps.size();i>0;--i)
                {
                    Prospect &pro = pps[i]; assert(Running==pro.st);
                    pro.ff = objFunc(pro.cc,SubLevel);
                }
                HeapSort::Call(pps,Prospect::CompareIncreasingFF);
                showProspects(xml,Ktop);

                if(pps.size()<=1)
                {
                    assert(1==pps.size());
                    assert(0==basis.size);
                    basis << pps[1];
                    return;
                }
            }


            //------------------------------------------------------------------
            //
            //
            // set common starting point and study each prospect
            //
            //
            //------------------------------------------------------------------
            ff0 = objGrad(mine.transfer(Cin, SubLevel, C, L),SubLevel);

            //------------------------------------------------------------------
            //
            //
            // select promising solutions
            //
            //
            //------------------------------------------------------------------
            {
                Y_XML_SECTION(xml,"xselect");
                Y_XMLOG(xml, "|               |" << Formatted::Get("%15.4f", real_t(ff0)) << "| = ff0");

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
                        // positive or zero slope, cancel this position
                        //------------------------------------------------------
                        //saveProfile(pro,50);
                        mine.transfer(pro.cc, SubLevel, C, L);
                        pro.dc.ld(pro.xi=0);
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
                        // apply minimize
                        //------------------------------------------------------
                        const xreal_t    uopt = Minimize<xreal_t>::Locate(Minimizing::Inside, fcn, uu, ff);
                        const xreal_t    Fopt = fcn(uopt);

                        //------------------------------------------------------
                        // update status: cc and xi
                        //------------------------------------------------------
                        pro.ff = Fopt;
                        pro.cc.ld(Cws);
                        pro.ax = (pro.xi = afm.eval(pro.dc, pro.cc, SubLevel, Cin, SubLevel, pro.eq)).abs();
                        //saveProfile(pro,100);
                    }


                }

                //--------------------------------------------------------------
                //
                // get new order
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
                Y_XMLOG(xml, "|               |" << Formatted::Get("%15.4f", real_t(ff0)) << "| = A0");
                for(const PNode *pn=basis.head;pn;pn=pn->next)
                {
                    (**pn).show(xml(), mine, 0) << std::endl;
                }
            }

        }

        void Solver:: process(XWritable &       C,
                              const Level       L,
                              const XReadable & Ktop,
                              XMLog           & xml)
        {

            Y_XML_SECTION(xml, "process");

            OutputFile::Overwrite(NRA_Step);
            OutputFile::Overwrite(ODE_Step);
            vfree();
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
                    case 0: throw Specific::Exception(CallSign, "not possible");

                    case 1: {
                        const Prospect &pro = pps.head();
                        mine.transfer(C, L, pro.cc, SubLevel);
                    } return;

                    default:
                        break;
                }

                assert(pps.size()>=2);


                const Prospect &pro = pps.head();
                vpush(pro.cc,pro.ff); // or another ?
                saveProfile(pro,1000);


                const bool hasNRA = nraStep(xml);
                const bool hasODE = odeStep(xml);


                {
                    Y_XML_SECTION_OPT(xml, "vlist", "size='" << vlist.size << "'");
                    assert(vlist.size>0);
                    MergeSort::Call(vlist, Vertex::Compare);
                    for(const Vertex *v=vlist.head;v;v=v->next)
                    {
                        std::cerr << Formatted::Get("%15.4f",real_t(v->cost)) << " @C=" << *v << std::endl;
                    }
                    const Vertex &ans = *vlist.head;
                    mine.transfer(C,L,ans, SubLevel);
                }

                std::cerr << "plot '" << pro.fileName() << "' w l ls 1";

                if(hasNRA) {
                    std::cerr << ", '" << NRA_Step << "' w l ls 2";
                }

                if(hasODE) {
                    std::cerr << ", '" << ODE_Step << "' w l ls 3";
                }

                std::cerr << std::endl;

            }
        }



    }

}


