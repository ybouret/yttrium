
#include "y/chemical/plexus/reactor.hpp"
#include "y/mkl/opt/minimize.hpp"

#include "y/stream/libc/output.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        using namespace MKL;

        Reactor:: Reactor(const Cluster &persistentCluster) :
        cluster(persistentCluster),
        solve1D(),
        x_score(),
        running(),
        basis(),
        Ceq(cluster->equilibria->size,cluster->species->size),
        Cini(cluster->species->size),
        Cend(cluster->species->size),
        Ctry(cluster->species->size),
        qMetrics(cluster->species->size),
        qVCache( new QVector::Cache(qMetrics) ),
        qFamily( qVCache ),
        rate(cluster->species->size)
        {
            running.proxy->reserve(Ceq.rows);
        }

        Reactor:: ~Reactor() noexcept
        {
        }

        bool Reactor:: IsRunning(const Outcome &out) noexcept
        {
            assert(Blocked!=out.st);
            return Running == out.st;
        }


        xreal_t Reactor:: score(const XReadable &C,
                                const Level      L)
        {
            assert(running.size>0);
            x_score.free();
            for(const OutNode *node=running.head;node;node=node->next)
            {
                const xreal_t aff = (**node).affinity(solve1D.xadd,C,L);
                x_score << Squared(aff);
            }
            const xreal_t numer = x_score.sum();
            const xreal_t denom = running.size;
            const xreal_t ratio = numer/denom;
            return ratio.sqrt();
        }


        xreal_t Reactor:: operator()(const xreal_t u)
        {

            const xreal_t v   = xreal_t(1)-u;

            for(size_t i=Ctry.size();i>0;--i)
            {
                const xreal_t cv = Cini[i];
                const xreal_t cu = Cend[i];
                xreal_t       cmin = cv, cmax=cu;
                if(cmax<cmin)
                    Swap(cmin,cmax);
                assert(cmin<=cmax);
                Ctry[i] = Clamp(cmin,cv*v+cu*u,cmax);
            }

            return score(Ctry,SubLevel);
        }

        SignType Reactor:: ByDecreasingAX(const OutNode * const lhs,
                                          const OutNode * const rhs) noexcept
        {
            return Sign::Of( (**rhs).ax, (**lhs).ax );
        }

        SignType Reactor:: ByIncreasingSC(const OutNode * const lhs,
                                          const OutNode * const rhs) noexcept
        {
            return Sign::Of( (**lhs).sc, (**rhs).sc );
        }



        void Reactor:: initialize(XMLog &           xml,
                                  XWritable &       C0,
                                  const XReadable & K0)
        {
            size_t cycle = 0;
        CYCLE:
            ++cycle;
            Y_XML_SECTION_OPT(xml,"Initialize", "cycle="<<cycle);
            {
                running.free();
                bool emergency = false;
                for(const ENode *en=cluster->equilibria->head;en;en=en->next)
                {
                    const Equilibrium & eq  = **en;
                    const xreal_t       eK  = eq(K0,TopLevel);
                    XWritable        &  cc  = cluster.gather(Ceq[running.size+1],C0);
                    const Outcome       out = solve1D(eq,eK,cc,SubLevel,C0,TopLevel);
                    switch(out.st)
                    {
                        case Blocked:
                            Y_XMLOG(xml,out);
                            continue;

                        case Crucial:
                            Y_XMLOG(xml,out);
                            emergency = true;
                            running << out;
                            continue;

                        case Running:
                            if(emergency) continue; // discard in case or emergency
                            Y_XMLOG(xml,out);
                            running << out;
                            continue;
                    }
                }
                if(emergency)
                {
                    MergeSort::Call(running.removeIf(IsRunning),ByDecreasingAX);
                    const Outcome &worst = **running.head;
                    Y_XML_COMMENT(xml,"using crucial " << worst.eq.name);
                    cluster.expand(C0,worst.cc);
                    if(xml.verbose)
                    {
                        cluster.show(*xml << "C0=", TopLevel,"\t[", C0, "]", xreal_t::ToString) << std::endl;
                    }
                    goto CYCLE;
                }
            }
            Y_XML_COMMENT(xml, "|active| = " << running.size << "/" << cluster->equilibria->size);
            cluster.gather(Cini,C0);
        }

        real_t Reactor:: optimize1D(const xreal_t Sini)
        {
            Reactor &self = *this;
            XTriplet U    = { 0,    0,  1 };
            XTriplet S    = { Sini, 0,  score(Cend,SubLevel) };
            const xreal_t Uopt = Minimize<xreal_t>::Locate(Minimizing::Inside,self,U,S);
            const xreal_t Sopt = self(Uopt);
            return Sopt;
        }


        void Reactor:: ameliorate(XMLog &xml)
        {
            Y_XML_SECTION(xml, "Ameliorate");

            const xreal_t S0 = score(Cini,SubLevel);
            Y_XMLOG(xml, "S0=" << S0.str());

            //------------------------------------------------------------------
            //
            //
            // optimize each 1D
            //
            //
            //------------------------------------------------------------------
            for(OutNode *node=running.head;node;node=node->next)
            {
                Outcome &out = **node;

                //--------------------------------------------------------------
                //
                // Cend = cc
                //
                //--------------------------------------------------------------
                cluster.transfer(Cend,SubLevel,out.cc, out.lv);

                if(false)
                {
                    OutputFile fp(*out.eq.name+".pro");
                    const size_t np=100;
                    for(size_t i=0;i<=np;++i)
                    {
                        const real_t u = real_t(i)/np;
                        const real_t f = real_t((*this)(u));
                        fp("%.15g %.15g\n",u,f);
                    }
                }

                //--------------------------------------------------------------
                //
                // global optimize on Cini:Cend
                //
                //--------------------------------------------------------------
                const  xreal_t Stry = optimize1D(S0);

                if(Stry<S0)
                {
                    //----------------------------------------------------------
                    // better: update
                    //----------------------------------------------------------
                    cluster.transfer(out.cc,out.lv,Ctry,SubLevel);
                    out.xi = out.eq.extent(x_score, out.cc, out.lv, Cini, SubLevel);
                    out.ax = out.xi.abs();
                    out.sc = Stry;
                    Y_XMLOG(xml,"[*] " << out);
                }
                else
                {
                    //--------------------------------------------------------------
                    // not better: pinned down
                    //--------------------------------------------------------------
                    cluster.transfer(out.cc,out.lv,Cini,SubLevel);
                    out.xi = 0;
                    out.ax = 0;
                    out.sc = S0;
                    Y_XMLOG(xml,"[0] " << out);
                }
            }

            //------------------------------------------------------------------
            //
            //
            // build local basis
            //
            //
            //------------------------------------------------------------------
            Y_XML_COMMENT(xml, "Extract Basis");
            MergeSort::Call(running,ByIncreasingSC);
            const size_t dof = cluster.N;
            qFamily.clear();
            basis.free();
            for(const OutNode *node=running.head;node;node=node->next)
            {
                const Equilibrium   &eq = (**node).eq;
                const Readable<int> &nu = eq(cluster.Nu,SubLevel);
                if(qFamily.welcomes(nu))
                {
                    (void) qFamily.increase();
                    basis << eq;
                    if(qFamily->size>=dof) break;
                }
            }


            if(xml.verbose)
            {
                Y_XML_COMMENT(xml, "Snapshot");
                for(const OutNode *node=running.head;node;node=node->next)
                {
                    const Equilibrium &eq = (**node).eq;
                    if(basis.has(eq)) {
                        Y_XMLOG(xml, "(*) " << **node);
                    }
                    else {
                        Y_XMLOG(xml, "    " << **node);
                    }
                }
            }

        }

        void Reactor:: operator()(XMLog &           xml,
                                  XWritable &       C0,
                                  const XReadable & K0)
        {
            Y_XML_SECTION(xml, "Reactor");
            initialize(xml,C0,K0); if(running.size<=0) { Y_XML_COMMENT(xml, "All Blocked"); return; }
            ameliorate(xml);
        }



        void Reactor:: queryRates(XMLog &xml)
        {
            Y_XML_SECTION(xml,"QueryRates");
            rate.forEach( & XAdd::free );
            for(const OutNode *node=running.head;node;node=node->next)
            {
                
            }

        }

    }

}
