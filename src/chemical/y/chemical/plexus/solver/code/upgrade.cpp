

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
            // initialize all
            //
            //
            //------------------------------------------------------------------
            ff0 = 0;
            vlist.free();
            ortho.free();
            basis.free();
            //bool hadEmergency = false;

            //------------------------------------------------------------------
            //
            //
            // looking for [running|crucial] solutions
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

                        case Crucial:
                            emergency    = true;
                            //hadEmergency = true;
                            Y_XMLOG(xml, "(!) " << eq);
                            break;
                    }

                    const Prospect pro(st,eq,ek,cc,afm.eval(dc,cc,SubLevel,C,L,eq),dc);
                    pps << pro;
                }

                //--------------------------------------------------------------
                //
                // special case: all blocked, won't move anymore. Ever...
                //
                //--------------------------------------------------------------
                if( pps.size() <= 0 ) {
                    Y_XML_COMMENT(xml,"all jammed");
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

            //------------------------------------------------------------------
            //
            //
            // at this point, only (at least one) Running prospect(s).
            //
            //------------------------------------------------------------------
            assert(pps.size()>=1);
            assert(0==ortho.size);
            assert(0==basis.size);
            assert(0==vlist.size);

            //------------------------------------------------------------------
            //
            //
            // set common starting point for all algorithms:
            // - set     Cin as starting point
            // - set     grd as gradient to check all validities
            // - set     ff0 from all active prospects
            //
            //------------------------------------------------------------------
            ff0 = objGrad(mine.transfer(Cin,SubLevel,C,L),SubLevel);
            const size_t npro = pps.size();
            Y_XML_COMMENT(xml," #prospect = " << npro << ", ff0=" << real_t(ff0) );

            //------------------------------------------------------------------
            //
            // in any case, compute ff and check full valid
            //
            //------------------------------------------------------------------
            for(size_t i=npro;i>0;--i)
            {
                Prospect &pro = pps[i]; assert(Running==pro.st);
                pro.ff = objFunc(pro.cc,SubLevel);
                pro.eq.mustSupport(C,L);
                for(size_t j=npro;j>0;--j)
                {
                    pps[j].eq.mustSupport(pro.cc, SubLevel);
                }
            }

            switch( npro )
            {

                case 0:   // corrupted code :(
                    throw Specific::Exception(CallSign,"corrupted code");

                case 1: { // only one running prospect
                    Y_XML_SECTION(xml, "single");
                    Prospect &pro = pps.head();
                    showProspects(xml,Ktop);
                    return accepts(pro) ? 1 : 0; // good/bad single
                }
                default:
                    break;
            }

            assert(npro>=2);


            size_t good = 0;
            {
                Y_XML_SECTION_OPT(xml,"enhance", "count='" << npro << "'");
                Y_XMLOG(xml, "|               |" << Formatted::Get("%15.4g", real_t(ff0)) << "| = ff0");
                //--------------------------------------------------------------
                //
                // optimize each
                //
                //--------------------------------------------------------------
                for(size_t i=npro;i>0;--i)
                    if(enhance(pps[i])) ++good;

                //--------------------------------------------------------------
                //
                // order them to build basis
                //
                //--------------------------------------------------------------
                HeapSort::Call(pps,Prospect::CompareIncreasingFF);
                showProspects(xml,Ktop);
                Y_XML_COMMENT(xml, "good = " << good << " / " << npro);
            }


            //------------------------------------------------------------------
            //
            //
            // select family/basis of running prospects for NRA
            //
            //
            //------------------------------------------------------------------
            for(size_t i=1;i<=npro;++i)
            {
                const Prospect    &   pro = pps[i]; assert(Running==pro.st);
                const Equilibrium &   eq  = pro.eq;
                const size_t          ei  = eq.indx[SubLevel];
                const Readable<int> & nu  = mine.iTopo[ei];

                if(ortho.wouldAccept(nu))
                {
                    ortho.expand();
                    basis << pro;
                    if(ortho.size>=dof)
                        break; // basis is full in any case
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

            return good;

        }



    }

}

