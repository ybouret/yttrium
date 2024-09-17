
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

        bool Solver:: process(XWritable &       C,
                              const Level       L,
                              const XReadable & Ktop,
                              XMLog           & xml)
        {
            assert(0==vlist.size);

            OutputFile::Overwrite(NRA_Step);
            OutputFile::Overwrite(ODE_Step);

            const size_t good = upgrade(C,L,Ktop,xml); assert( good <= pps.size() );
            const size_t npro = pps.size();

            {
                Y_XML_SECTION(xml, "process");
                //--------------------------------------------------------------
                //
                //
                // evolve
                //
                //
                //--------------------------------------------------------------
                switch( npro )
                {
                    case 0:
                        //------------------------------------------------------
                        // jammed
                        //------------------------------------------------------
                        assert(0==good);
                        return false;

                    case 1: {
                        //------------------------------------------------------
                        // single solution, even if good = 0
                        //------------------------------------------------------
                        const Prospect &pro = pps.head();
                        mine.transfer(C, L, pro.cc, SubLevel);
                    } return true;

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
                const Prospect &pro = pps.head();
                if(good>0)
                {
                    vlist.push(pro.cc,pro.ff).info = MIN_Step; // or another ?
                    saveProfile(pro,1000);
                }

                const bool hasNRA = nraStep(xml); // then the best NRA step
                const bool hasODE = odeStep(xml); // then the best ODE step

                Y_XMLOG(xml, "#vertices = " << vlist.size);
                if(vlist.size<=0)
                {
                    return false;
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
                    const Vertex &ans = *vlist.head;
                    mine.transfer(C,L,ans, SubLevel);
                }


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
            }

            return true;
        }





    }

}


