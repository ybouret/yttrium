#include "y/chemical/plexus/device.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Device:: ~Device() noexcept {}

        Device:: Device(const Cluster &_, const bool doTrace) :
        Joint(_),
        aftermath(),
        EqConc(neqs,nspc),
        EqDiff(neqs,nspc),
        ansatz(neqs),
        Cini(nspc),
        Cend(nspc),
        Ctmp(nspc),
        Copt(nspc),
        dC(nspc),
        ff0(),
        ff1(),
        objValue(neqs),
        gradient(nspc),
        increase(nspc),
        dof(mine.Nu.rows),
        ortho(nspc,dof),
        basis(dof),
        shield(0.99),
        xPhi(dof),
        xNu(dof),
        xChi(dof),
        xXi(dof),
        xlu(dof),
        trace(doTrace)
        {
            for(size_t i=1;i<=dof;++i)
            {
                xPhi. grow(i,nspc);
                xNu.  grow(i,nspc);
                xChi. grow(i,i);
                xXi.  grow(i);
            }
        }

        xreal_t Device:: objectiveFunction(const XReadable &C, const Level L)
        {
            objValue.free();
            XMul &X = aftermath.xmul;
            for(size_t i=ansatz.size();i>0;--i)
                objValue << ansatz[i].objectiveFunction(X,C,L);
            return aftermath.xadd.normOf(objValue);
        }

        xreal_t Device:: operator()(const xreal_t u)
        {
            const xreal_t one(1);
            const xreal_t v = one-u;
            for(size_t i=nspc;i>0;--i)
            {
                const xreal_t c0 = Cini[i];
                const xreal_t c1 = Cend[i];
                xreal_t cmin = c0, cmax = c1;
                if(cmax<cmin) Swap(cmin,cmax);
                Ctmp[i] = Clamp(cmin, v*c0 + u * c1, cmax);
            }
            return objectiveFunction(Ctmp,SubLevel);
        }

        xreal_t Device:: objectiveGradient(const XReadable &C,
                                           const Level      L)
        {
            const xreal_t _0;
            increase.forEach( &XAdd::free );
            objValue.free();
            gradient.ld(_0);


            const size_t m  = nspc;

            {
                XWritable & dA = Ctmp;
                XMul      &  X = aftermath.xmul;
                for(size_t i = ansatz.size();i>0;--i)
                {
                    const Ansatz      &ans = ansatz[i];
                    const Equilibrium &eq  = ans.eq;
                    const xreal_t      A   = ans.objectiveFunction(X,C,L);
                    eq.drvsAffinity(dA, SubLevel, C, L);
                    for(size_t j=m;j>0;--j)
                    {
                        increase[j] << A * dA[j];
                    }
                    objValue << A;
                }
            }

            const xreal_t den = aftermath.xadd.normOf(objValue);
            if(den.mantissa>0)
            {
                for(size_t j=m;j>0;--j)
                {
                    gradient[j] = increase[j].sum() / den;
                }
            }

            return den;
        }


        void Device:: showAnsatz(XMLog &xml) const
        {
            if(!xml.verbose) return;
            const size_t na = ansatz.size();
            Y_XML_SECTION_OPT(xml, "Ansatz", "count=" << ansatz.size() << "/" << neqs);
            for(size_t i=1;i<=na;++i)
            {
                xml() << ansatz[i] << std::endl;
            }
        }


        void Device:: computeRate(XWritable &rate)
        {

            //------------------------------------------------------------------
            //
            // initialize increases
            //
            //------------------------------------------------------------------
            increase.forEach( &XAdd::free );

            //------------------------------------------------------------------
            //
            // use Ansatz to compute increases
            //
            //------------------------------------------------------------------
            for(size_t i=ansatz.size();i>0;--i)
            {
                const Ansatz &ans = ansatz[i];
                if(!ans.ok) continue; // shortcut for zero
                ans.step(increase);
            }

            //------------------------------------------------------------------
            //
            // deduce rate
            //
            //------------------------------------------------------------------
            for(const SNode *sn=mine.species.head;sn;sn=sn->next)
            {
                const size_t j = (**sn).indx[ SubLevel ];
                rate[j] = increase[j].sum();
            }

        }

    }

}

#include "y/stream/libc/output.hpp"
#include "y/system/exception.hpp"
#include "y/sort/heap.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        void Device::  operator()(XWritable       &C,
                                  const Level      L,
                                  const XReadable &K,
                                  XMLog           &xml)
        {

            AutoPtr<OutputStream> fp = (trace ? new OutputFile("ff.dat") : 0);

            //------------------------------------------------------------------
            //
            //
            // processing up to convergence
            //
            //
            //------------------------------------------------------------------
            for(unsigned long cycle=1;;++cycle)
            {
                const Outcome outcome = process(C,L,K,xml);

                if(fp.isValid())
                {
                    if(1==cycle) (*fp)("0 %.15g\n", double(ff0));
                    (*fp)("%lu %.15g\n", cycle, double(ff1));
                }

                switch(outcome)
                {
                    case Jammed: goto CONVERGED;
                    case Solved: goto CONVERGED;
                    case Better:
                        if(ff1>=ff0)
                        {
                            Y_XML_COMMENT(xml,"minimum was reached");
                            goto CONVERGED;
                        }
                        break;
                }
            }

            //------------------------------------------------------------------
            //
            //
            // build final basis
            //
            //
            //------------------------------------------------------------------
        CONVERGED:
            {
                const size_t na = ansatz.size();
                Y_XML_SECTION_OPT(xml, "Basis", "ansatz='" << na << "' neqs='" << neqs << "'" << " dof='" << dof << "'");
                basis.free();
                ortho.free();
                HeapSort::Call(ansatz, Ansatz::NaturalOrder);

                for(size_t i=1;i<=na;++i)
                {
                    const Ansatz        & ans = ansatz[i];
                    if( ortho.wouldAccept( mine.iTopo[ ans.eq.indx[SubLevel] ] ) )
                    {
                        Y_XMLOG(xml, "(+) " << ans.eq);
                        ortho.expand();
                        if( (basis << ans).size >= dof )
                            break;
                    }
                }
                assert( basisOkWith(C,L) );
            }

        }

    }

}



