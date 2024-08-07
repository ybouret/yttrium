#include "y/chemical/plexus/solver/normalizer.hpp"

#include "y/mkl/opt/minimize.hpp"
#include "y/stream/libc/output.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        using namespace MKL;
        


        xreal_t  Normalizer:: improve(XWritable       & Ctop,
                                      const bool        repl,
                                      XMLog           & xml)
        {
            Y_XML_SECTION(xml, "Improve");
            assert(apl.size>0);

            {
                Y_XML_SECTION(xml, "Simplex");
                //--------------------------------------------------------------
                //
                //
                // load simplex with optional Ctop + basis
                //
                //
                //--------------------------------------------------------------
                sim.free();
                if(!repl)
                {
                    const xreal_t val = sim.load( objectiveFunction(Ctop, TopLevel), rcl.species, Ctop, TopLevel).cost;
                    Y_XMLOG(xml, std::setw(15) << real_t(val) << " @Ctop");
                }
                else
                {
                    Y_XMLOG(xml, "[replaced Ctop]");
                }

                for(const ANode *an=apl.head;an;an=an->next)
                {
                    const Applicant &app = **an;
                    const xreal_t    val = sim.load( objectiveFunction(app.cc, SubLevel), rcl.species, app.cc, SubLevel).cost;
                    Y_XMLOG(xml,std::setw(15) << real_t(val) << " @" << app.eq);
                }

            }

            {
                Y_XML_SECTION(xml, "Search");
                unsigned long cycle = 0;
                const size_t  m     = nsp;
                OutputFile fp("objective.dat");

                while(sim.size>1)
                {
                    ++cycle;
                    //Y_XMLOG(xml, "#cycle = " << cycle);
                    Triplet<xreal_t> xx = {  0, -1,  1 };
                    Triplet<xreal_t> ff = { -1, -1, -1 };

                    //----------------------------------------------------------
                    //
                    //
                    // extract upper/lower and load Cin/Cex
                    //
                    //
                    //----------------------------------------------------------
                    xreal_t up,lo;
                    {
                        AutoPtr<Vertex> upper = sim.query();
                        AutoPtr<Vertex> lower = sim.query();

                        up = ff.a = upper->cost;
                        lo = ff.c = lower->cost;

                        for(size_t j=m;j>0;--j)
                        {
                            Cin[j] = (*upper)[j];
                            Cex[j] = (*lower)[j];
                        }
                        sim.free( upper.yield() );
                        sim.free( lower.yield() );
                    }

                    //Y_XMLOG(xml, "upper = " << std::setw(15) << real_t(ff.a) );
                    //Y_XMLOG(xml, "lower = " << std::setw(15) << real_t(ff.c) );
                    assert(ff.a>=ff.c);

                    if(false)
                    {
                        const real_t offset = cycle-1;
                        const size_t np(1000);
                        for(size_t j=0;j<=np;++j)
                        {
                            const real_t u = real_t(j)/np;
                            fp("%.15g %.15g\n", offset+u,  (real_t( (*this)(u) ) ) );
                        }
                        fp << "\n";
                    }

                    //----------------------------------------------------------
                    //
                    //
                    // Minimize between upper and lower
                    //
                    //
                    //----------------------------------------------------------
                    const xreal_t u_opt = Minimize<xreal_t>::Locate(Minimizing::Inside, *this, xx, ff);
                    const xreal_t cost  = (*this)(u_opt);
                    sim.load(cost,rcl.species,Cws,SubLevel);
                    Y_XMLOG(xml, "optim = " << std::setw(15) << real_t(cost)  <<  " @" << std::setw(15) << real_t(u_opt) << " | " << real_t(up) << "->" << real_t(lo) );
                }

            }



            rcl.transfer(Ctop, TopLevel, Cws, SubLevel);

            return sim.head->cost;
        }
    }

}


