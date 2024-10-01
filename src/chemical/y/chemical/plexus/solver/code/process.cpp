
#include "y/chemical/plexus/solver.hpp"

#include "y/sort/heap.hpp"
#include "y/system/exception.hpp"
#include "y/mkl/opt/minimize.hpp"

#include "y/jive/pattern/vfs.hpp"
#include "y/vfs/local/fs.hpp"
#include "y/stream/libc/output.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        using namespace MKL;

#define SOLVER_RETURN(FLAG) Y_XML_COMMENT(xml, #FLAG); return FLAG

        Outcome Solver:: process(XWritable &       C,
                                 const Level       L,
                                 const XReadable & Ktop,
                                 XMLog           & xml)
        {
            Y_XML_SECTION(xml, "process");

            OutputFile::Overwrite(NRA_Step);
            OutputFile::Overwrite(ODE_Step);

            //------------------------------------------------------------------
            //
            //
            // query current prospects
            //
            //
            //------------------------------------------------------------------
            const size_t good = upgrade(C,L,Ktop,xml); assert( good <= pps.size() );
            const size_t npro = pps.size();
            assert(vlist.size==0);

            //------------------------------------------------------------------
            //
            //
            // evolve
            //
            //
            //------------------------------------------------------------------
            switch( npro )
            {
                case 0:
                    //----------------------------------------------------------
                    // jammed => nothing to do
                    //----------------------------------------------------------
                    assert(0==good);
                    SOLVER_RETURN(Jammed);

                case 1: {
                    //----------------------------------------------------------
                    // single solution, even if good = 0 (numerical slope >= 0)
                    //----------------------------------------------------------
                    const Prospect &pro = pps.head();
                    mine.transfer(C, L, pro.cc, SubLevel);
                }  SOLVER_RETURN(Solved);

                default:
                    break;
            }

            assert(npro>=2);

            //--------------------------------------------------------------
            //
            //
            // initialize vlist with the best equilibrium
            //
            //
            //--------------------------------------------------------------
            const Prospect &pro    = pps.head(); // even if not ok
            const bool      hasMIN = good>0;
            if(hasMIN)
            {
                assert(pro.ok);
                vlist.push(pro.cc,pro.ff).info = MIN_Step; // or another ?
            }
            saveProfile(pro,1000);

            //--------------------------------------------------------------
            //
            //
            // Look for solutions
            //
            //
            //--------------------------------------------------------------
            const bool hasNRA = nraStep(xml); // then the best NRA step
            const bool hasODE = odeStep(xml); // then the best ODE step

            Y_XMLOG(xml, "#vertices = " << vlist.size);
            if(vlist.size<=0) {
                SOLVER_RETURN(Locked);
            }

            //--------------------------------------------------------------
            //
            //
            // then sort collected vertices
            //
            //
            //--------------------------------------------------------------
            {
                Y_XML_SECTION_OPT(xml, "vlist", "size='" << vlist.size << "'");
                assert(vlist.size>0);
                MergeSort::Call(vlist, Vertex::Compare);
                if(xml.verbose)
                {
                    xml() << Formatted::Get("%15.4g",real_t(ff0)) << " = ff0" << std::endl;
                    for(const Vertex *v=vlist.head;v;v=v->next)
                    {
                        xml() << Formatted::Get("%15.4g",real_t(v->cost));
                        if(v->info)
                        {
                            *xml << " [" << v->info << "]";
                        }
                        *xml << " @C=" << *v << std::endl;
                    }
                }

            }



            //--------------------------------------------------------------
            //
            //
            // And transfer most promising
            //
            //
            //--------------------------------------------------------------
            {
                const Vertex &ans = *vlist.head;
                mine.transfer(C,L,ans, SubLevel);
            }

            // helper for monitoring
            if(xml.verbose)
            {

                xml() << std::endl << "plot '" << pro.fileName() << "' w l ls 1";


                if(hasNRA) {
                    *xml << ", '" << NRA_Step << "' w l ls 2";
                }

                if(hasODE) {
                    *xml<< ", '" << ODE_Step << "' w l ls 3";
                }

                *xml << std::endl << std::endl;
            }

            if( vlist.head->cost.abs().mantissa <= 0.0 )
            {
                // numerical solution
                SOLVER_RETURN(Solved);
            }
            else
            {
                // numerical improvement
                SOLVER_RETURN(Better);
            }
        }



    }

}


