#include "y/chemical/plexus/solver.hpp"
#include "y/chemical/plexus/mix.hpp"
#include "y/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        xReal Solver:: computeStepAS(XMLog &xml, const xReal f0)
        {
            static const char fn[] ="AlgebraicSystem";
            Y_XML_SECTION(xml,fn);

            //------------------------------------------------------------------
            //
            //
            // extract orthogonal basis
            //
            //
            //------------------------------------------------------------------
            assert(isAcceptable(Cini,SubLevel));
            getOrthoBasis(xml);

            //------------------------------------------------------------------
            //
            //
            // computing local algebra
            //
            //
            //------------------------------------------------------------------
            const size_t n = basis.size;
            //const size_t m = mix->species.size;
            XArray       & xi  = xiArr[n];
            XMatrix      & Phi = PhiMat[n]; //XMatrix      Phi(n,m);
            XMatrix      & Nu  = NuMat[n];  //XMatrix      Nu(n,m);
            XMatrix      & NuT = NuTMat[n]; //XMatrix      NuT(m,n);
            XMatrix      & Xi  = XiMat[n];  //XMatrix      Xi(n,n);

            for(const ProNode *pn=basis.head;pn;pn=pn->next)
            {
                const Prospect   &   pro = **pn;
                const Components &   eq  = pro.eq;
                const size_t * const id  = eq.indx;
                const size_t         ei  = id[AuxLevel];

                xi[ei] = -eq.score(xmul, pro.eK, Cini, SubLevel);
                eq.jacobian(Phi[ei], SubLevel, Cini, SubLevel);
                Nu[ei].ld( mix.topology[ id[SubLevel] ] );
            }
            NuT.assign(TransposeOf,Nu);

            //Y_XMLOG(xml, "rhs=" << xi);
            //Y_XMLOG(xml, "Phi=" << Phi);
            //Y_XMLOG(xml, "Nu="  << Nu);
            //Y_XMLOG(xml, "NuT="  << NuT);

            //------------------------------------------------------------------
            //
            //
            // preparing system matrix
            //
            //
            //------------------------------------------------------------------
            for(size_t i=n;i>0;--i)
            {
                XWritable &      Xi_i  = Xi[i];
                const XReadable &Phi_i = Phi[i];
                for(size_t j=n;j>0;--j)
                    Xi_i[j] = xadd.dot(Phi_i, Nu[j]);
            }

            Y_XMLOG(xml,"Xi="<<Xi);

            //------------------------------------------------------------------
            //
            //
            // computing extent
            //
            //
            //------------------------------------------------------------------
            if( !lu.build(Xi) )
            {
                Y_XML_COMMENT(xml, "singular system");
                return f0; // register as failure..
            }

            lu.solve(Xi,xi);
            Y_XMLOG(xml, "xi=" << xi);


            //------------------------------------------------------------------
            //
            //
            // computing step
            //
            //
            //------------------------------------------------------------------
            step.ld(zero);
            bool  mustCut = false;
            xReal scaling =     2; // default scaling

            for(const SNode *sn=mix->species.head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                const size_t   sj = sp.indx[SubLevel];
                const xReal    dc = step[sj] = xadd.dot(NuT[sj],xi);
                const xReal    cc = Cini[sj]; assert(cc>=0.0);


                if(dc<0.0)
                {
                    const xReal smax = cc/(-dc);
                    if(smax<scaling)
                    {
                        mustCut = true;
                        scaling = smax;
                    }
                }

                if(xml.verbose)
                {
                    mix->sformat.print(xml() << "d[", sp, Justify::Right)
                    << "] = " << std::setw(15) << real_t(dc)
                    << " / "  << std::setw(15) << real_t(cc)
                    << " => scaling=" << std::setw(15) << real_t(scaling)
                    << std::endl;
                }

            }

            if(mustCut) scaling *= safe;
            setRecentStep(scaling);
            while( !isAcceptable(Cend, SubLevel) )
            {
                scaling *= safe;
                setRecentStep(scaling);
            }
            save("nra.dat",100);

            //------------------------------------------------------------------
            //
            //
            // look for optimized position
            //
            //
            //------------------------------------------------------------------
            const xReal f1   = optimize(f0, objectiveFunction(Cend,SubLevel) );

            Y_XML_COMMENT(xml, fn << " scaling was " << real_t(scaling) );
            Y_XML_COMMENT(xml, fn << " function is " << real_t(f1) << " / " << real_t(f0) );

            return f1;
        }
    }

}

