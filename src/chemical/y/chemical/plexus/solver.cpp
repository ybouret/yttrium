

#include "y/chemical/plexus/solver.hpp"
#include "y/stream/libc/output.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const char * const Solver:: CallSign = "Chemical::Solver";
        const char * const Solver:: NRA_Step = "nraStep.dat";
        const char * const Solver:: ODE_Step = "odeStep.dat";
        const char * const Solver:: MIN_Step = "minStep.dat";


        Solver:: Solver(const Cluster &cl) :
        Joint(cl),
        afm(),
        ceq(neqs,nspc),
        deq(neqs,nspc),
        pps(neqs),
        dof(mine.Nu.rows),
        obj(neqs),
        ortho(nspc,dof),
        pbank(),
        basis(pbank),
        vlist(nspc),
        Cin(nspc),
        Cex(nspc),
        Cws(nspc),
        ddC(nspc),
        fcn(*this),
        ff0(0),
        grd(nspc),
        inc(nspc),
        xlu(dof),
        xsf(0.99),
        xPhi(dof),
        xNu(dof),
        xChi(dof),
        xXi(dof)
        {
            for(size_t i=1;i<=dof;++i)
            {
                xPhi. grow(i,nspc);
                xNu.  grow(i,nspc);
                xChi. grow(i,i);
                xXi.  grow(i);
            }
        }


        Solver:: ~Solver() noexcept {}

        void Solver:: showProspects(XMLog &xml, const XReadable &Ktop) const
        {
            if(xml.verbose)
            {
                for(size_t i=1;i<=pps.size();++i)
                {
                    pps[i].show( xml(), mine, &Ktop) << std::endl;
                }
            }
        }

        bool Solver:: basisOkWith(const XReadable &C, const Level L) const noexcept
        {
            for(const PNode *pn=basis.head;pn;pn=pn->next)
            {
                if( ! (**pn).eq.canTolerate(C,L) ) return false;
            }
            return true;
        }

        bool Solver:: stepWasCut(XWritable &       target,
                                 const XReadable & source,
                                 XWritable &       deltaC,
                                 xreal_t * const   result) const
        {

            //------------------------------------------------------------------
            //
            //
            // initialize
            //
            //
            //------------------------------------------------------------------
            real_t scale = 1.0;
            bool   abate = false;
            assert( basisOkWith(source,SubLevel) );
            assert(target.size()==source.size());
            assert(target.size()==deltaC.size());

            //------------------------------------------------------------------
            //
            //
            // loop over components, act on negative deltaC
            //
            //
            //------------------------------------------------------------------
            const size_t m = target.size();
            for(size_t j=m;j>0;--j)
            {
                const xreal_t d = deltaC[j];  if(d.mantissa>=0) continue;
                const xreal_t c = source[j];  assert(c.mantissa>=0);
                const xreal_t f = c/(-d);
                if(f<=scale)
                {
                    abate = true;
                    scale = f;
                }
            }

            //------------------------------------------------------------------
            //
            //
            // apply safety
            //
            //
            //------------------------------------------------------------------
            if( abate )
                scale *= xsf;

            //------------------------------------------------------------------
            //
            //
            // generate
            //
            //
            //------------------------------------------------------------------
        GENERATE:
            for(size_t j=m;j>0;--j)
                target[j] = source[j] + scale * deltaC[j];

            if( !basisOkWith(target,SubLevel) )
            {
                scale *= xsf;
                abate  = true;
                goto GENERATE;
            }

            if(result) *result = scale;

            //------------------------------------------------------------------
            //
            //
            // recompute effective step
            //
            //
            //------------------------------------------------------------------
            for(size_t j=m;j>0;--j)
            {
                deltaC[j] = target[j] - source[j];
            }


            return abate;
        }


        void Solver:: saveProfile(const String &fn, const size_t np)
        {
            OutputFile fp(fn);
            for(size_t i=0;i<=np;++i)
            {
                const real_t u = double(i)/np;
                fp("%.15g %.15g\n", u, real_t(fcn(u)));
            }
        }

        void Solver:: saveProfile(const Prospect &pro, const size_t np)
        {
            Cex.ld(pro.cc);
            const String fn = pro.fileName();
            saveProfile(fn,np);
        }

        

        void Solver:: run(XWritable &C, const Level L, const XReadable &Ktop, XMLog &xml)
        {
            Y_XML_SECTION(xml, "Solver::run" );
            // const size_t n = neqs;
            // const size_t m = nspc;

            // initial run
            Outcome outcome = process(C, L, Ktop, xml);
            switch( outcome )
            {
                case Jammed: return;
                case Solved: return;
                    
            }
        }



    }

}
