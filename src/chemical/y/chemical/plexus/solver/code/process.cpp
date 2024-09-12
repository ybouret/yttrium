
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

        void Solver:: process(XWritable &       C,
                              const Level       L,
                              const XReadable & Ktop,
                              XMLog           & xml)
        {

            Y_XML_SECTION(xml, "process");

            OutputFile::Overwrite(NRA_Step);
            OutputFile::Overwrite(ODE_Step);
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
                        return;

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


