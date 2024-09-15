
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

        void Solver:: process(XWritable &       C,
                              const Level       L,
                              const XReadable & Ktop,
                              XMLog           & xml)
        {
            assert(0==vlist.size);

            OutputFile::Overwrite(NRA_Step);
            OutputFile::Overwrite(ODE_Step);

            const size_t npro = upgrade(C,L,Ktop,xml);

            {
                Y_XML_SECTION(xml, "evolve");
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
                        return; // jammed

                    case 1: {
                        const Prospect &pro = pps.head();
                        mine.transfer(C, L, pro.cc, SubLevel);
                        //vlist.push(pro.cc,pro.ff).info = MIN_Step;
                    } return;

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
                vlist.push(pro.cc,pro.ff).info = MIN_Step; // or another ?
                saveProfile(pro,1000);

                const bool hasNRA = nraStep(xml); // then the best NRA step
                const bool hasODE = odeStep(xml); // then the best ODE step


                //--------------------------------------------------------------
                //
                //
                // initialize vlist with the best equilibrium
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

        }





    }

}


