#include "y/chemical/plexus/solver/normalizer.hpp"
#include "y/system/exception.hpp"
#include "y/text/boolean.hpp"
#include "y/mkl/opt/minimize.hpp"
#include "y/stream/libc/output.hpp"
#include "y/sort/heap.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        using namespace MKL;

        bool Normalizer:: fortify(XWritable &Ctop, const XReadable &Ktop, XMLog &xml)
        {
            Y_XML_SECTION(xml, "Fortify");

            //------------------------------------------------------------------
            //
            //
            // compile all active equilibria
            //
            //
            //------------------------------------------------------------------
            bool         repl = false;
            const size_t napp = compile(Ctop, Ktop, repl, xml);

            switch(napp)
            {
                case 0: Y_XMLOG(xml, "[Inactive] => false"); return false;
                case 1:
                    if(repl)
                    {
                        //------------------------------------------------------
                        //
                        // one active that replaced Ctop => true
                        //
                        //------------------------------------------------------
                        Y_XMLOG(xml, "[Replaced] => true");
                        return true;
                    }
                    else
                    {
                        //------------------------------------------------------
                        //
                        // check if strictly improved affinity
                        //
                        //------------------------------------------------------
                        const Applicant &app  = aps[1];
                        const XReadable &Csub = app.cc;
                        const xreal_t    aff0 = objectiveFunction(Ctop,TopLevel);
                        const xreal_t    aff1 = objectiveFunction(Csub,SubLevel);
                        const bool       flag = aff1<aff0;
                        Y_XMLOG(xml, "[" << aps[1].eq.name << "] => " << BooleanTo::text(flag) );
                        if(flag)
                            rcl.transfer(Ctop,TopLevel,Csub,SubLevel);

                        return flag;
                    }
                default:
                    break;

            }
            assert(napp>=2);


            //------------------------------------------------------------------
            //
            //
            // update applicant values
            //
            //
            //------------------------------------------------------------------


            {
                Y_XML_SECTION(xml, "Update");
                for(size_t i=napp;i>0;--i)
                {
                    const Applicant &app = aps[i];
                    app.ff = objectiveFunction(app.cc, SubLevel);
                }
                HeapSort::Call(aps,Applicant::CompareFF);
                for(size_t i=1;i<=napp;++i)
                {
                    const Applicant &app  = aps[i];
                    Y_XMLOG(xml, "[ff= " << std::setw(15)  << real_t(app.ff)
                            <<   "|ax= " << std::setw(15)  << real_t(app.ax)
                            <<   "] @" << app.eq.name);
                }
            }


            //------------------------------------------------------------------
            //
            //
            // extract basis from most promising
            //
            //
            //------------------------------------------------------------------
            const size_t dims = extract(xml);
            if(dims<=2) throw Specific::Exception(CallSign, "invalid fortified basis");

#if 0
            for(const ANode *an=apl.head;an;an=an->next)
            {
                const Applicant &app = **an;
                Y_XMLOG(xml, "[ff= " << std::setw(15) << real_t(app.ff)
                        <<   "|ax= " << std::setw(15)  << real_t(app.ax)
                        <<   "] @" << app.eq.name);
            }
#endif


            const SList &species = rcl.species;

            {
                Y_XML_SECTION(xml, "Simplex");
                //--------------------------------------------------------------
                //
                //
                // load simplex with optional Ctop
                //
                //
                //--------------------------------------------------------------
                sim.free();
                if(!repl)
                {
                    const xreal_t val = sim.load( objectiveFunction(Ctop, TopLevel), species, Ctop, TopLevel).cost;
                    Y_XMLOG(xml, "[ff= " << std::setw(15) << real_t(val) << "] @top-level");
                }
                else
                {
                    Y_XMLOG(xml, "[replaced top-level]");
                }

                //--------------------------------------------------------------
                //
                //
                // then load extracted basis
                //
                //
                //--------------------------------------------------------------
                for(const ANode *an=apl.tail;an;an=an->prev)
                {
                    const Applicant &app = **an;
                    const xreal_t    val = sim.load( objectiveFunction(app.cc, SubLevel), species, app.cc, SubLevel).cost;
                    Y_XMLOG(xml, "[ff= " << std::setw(15)  << real_t(val)
                            <<   "|ax= " << std::setw(15)  << real_t(app.ax)
                            <<   "] @" << app.eq.name);
                }

            }

            const xreal_t worst = sim.head->cost;
            const size_t  m     = nsp;

            {
                Y_XML_SECTION_OPT(xml, "Improve"," iter='" << sim.size-1 << "'");
                Normalizer & F     = *this;
                unsigned     cycle = 0;
                OutputFile   fp("simplex.dat");
                while(sim.size>1)
                {
                    ++cycle;
                    // load two highest vertices
                    Triplet<xreal_t> xx = {  0, -1,  1 };
                    Triplet<xreal_t> ff = { -1, -1, -1 };
                    {
                        const Vertex * const up = sim.head;
                        const Vertex * const lo = up->next;
                        ff.a = up->cost;
                        ff.c = lo->cost;
                        for(size_t j=m;j>0;--j)
                        {
                            Cin[j] = (*up)[j];
                            Cex[j] = (*lo)[j];
                        }
                        sim.pop().pop();
                    }

                    {
                        const size_t np = 1000;
                        for(size_t j=0;j<=np;++j)
                        {
                            const double u = double(j)/np;
                            fp("%.15g %.15g\n", u+double(cycle), real_t(F(u)));
                        }
                    }

                    // minimize between them
                    const xreal_t fmax = ff.a;
                    const xreal_t fmin = ff.c;
                    const xreal_t uopt = Minimize<xreal_t>::Locate(Minimizing::Inside, F, xx, ff);
                    const xreal_t cost = F(uopt);
                    Y_XMLOG(xml, "[ff= " << std::setw(15) << real_t(cost)
                            <<   " @"    << std::setw(15) << real_t(uopt)
                            <<   "] "    << std::setw(15) << real_t(fmax) << " -> " << std::setw(15) << real_t(fmin) << " / #" << cycle );

                    // replace with new value
                    sim.load(cost, species, Cws, SubLevel);
                }

            }

            const xreal_t issue = sim.head->cost;
            Y_XMLOG(xml, "affinity: " << real_t(worst) << " -> " << real_t(issue));
            if(issue<worst)
            {
                Y_XMLOG(xml,Core::Success);
                rcl.transfer(Ctop,TopLevel, *sim.head, SubLevel);
                return true;
            }
            else
            {
                Y_XMLOG(xml,Core::Failure);
                return false;
            }
        }

    }

}


