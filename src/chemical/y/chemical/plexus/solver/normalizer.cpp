
#include "y/chemical/plexus/solver/normalizer.hpp"
#include "y/mkl/opt/minimize.hpp"
#include "y/system/exception.hpp"
#include "y/stream/libc/output.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        using namespace MKL;

        const char * const Normalizer:: CallSign = "Chemical::Normalizer";

        Normalizer:: Normalizer(const Cluster &cl) :
        rcl(cl),
        lek(rcl),
        nsp(cl.species.size),
        afm(),
        ceq(cl.size,nsp),
        aps(cl.size),
        obj(cl.size),
        Cin(nsp,0),
        Cex(nsp,0),
        Cws(nsp,0),
        dof(cl.Nu.rows),
        bnk(),
        apl(bnk),
        qfm(nsp,cl.size),
        sim(cl.size,nsp),
        pro(nsp,nsp),
        psf(1),
        Phi(dof),
        XNu(dof),
        Chi(dof),
        Lhs(dof)
        {
            // building auxiliary matrices
            for(size_t i=1;i<=dof;++i)
            {
                Phi.grow(i,nsp);
                XNu.grow(i,nsp);
                Chi.grow(i,i);
                Lhs.grow(i);
            }

            // create projection matrix
            setProj();
        }


        Normalizer:: ~Normalizer() noexcept
        {
        }

#if 0
        SignType Normalizer:: CompareBasis(const ANode * const lhs, const ANode * const rhs) noexcept
        {
            return Sign::Of( (**lhs).eq.indx[TopLevel], (**rhs).eq.indx[TopLevel] );
        }
#endif
        
        const Normalizer:: KeyType & Normalizer:: key() const noexcept { return lek; }


        void Normalizer:: run(XWritable       & Ctop,
                              const XReadable & Ktop,
                              XMLog           & xml)
        {
            Y_XML_SECTION_OPT(xml, "Normalizer ", " size='" << rcl.size << "' species='" << rcl.species.size << "'");

            xreal_t xaff = -1;
            while(true)
            {
                bool         repl = false;
                const size_t napp = compile(Ctop,Ktop,repl,xml);
                if(napp<=0) { Y_XMLOG(xml, "[Jammed!]"); return; }
                xaff = overall(Ctop,repl,xml);
                if(repl) continue;
                break;
            }



#if 0
            bool    repl = false;
            size_t  nmax = compile(Ctop, Ktop, repl, xml); if(nmax<=0) { Y_XMLOG(xml, "[Jammed!]"); return; }
            xreal_t ham0 = overall(Ctop, repl, xml);
            Y_XMLOG(xml, "found " << real_t(ham0));


            for(size_t iter=0;iter<1;++iter)
            {
                nmax = compile(Ctop, Ktop, repl, xml); if(nmax<=0) { Y_XMLOG(xml, "[Jammed!]"); return; }
                const xreal_t ham1= overall(Ctop, repl, xml);
                Y_XMLOG(xml, "found " << real_t(ham1));
            }

            
            NDSolve(Ctop, Ktop, xml);
#endif

        }

        xreal_t  Normalizer:: operator()(const xreal_t u)
        {
            const xreal_t one = 1;
            const xreal_t v   = one - u;

            for(size_t j=nsp;j>0;--j)
            {
                const xreal_t c0   = Cin[j];
                const xreal_t c1   = Cex[j];
                xreal_t       cmin = c0;
                xreal_t       cmax = c1;
                if(cmax<cmin) Swap(cmin,cmax);
                Cws[j] = Clamp(cmin,v*c0+u*c1,cmax);
            }

            return objectiveFunction(Cws,SubLevel);
        }

        xreal_t Normalizer:: objectiveFunction(const XReadable &C, const Level L)
        {
            obj.free();
            for(const ANode *an=apl.head;an;an=an->next)
                obj << (**an).affinity(afm.xmul,C,L);
            return afm.xadd.normOf(obj);
        }

        void Normalizer:: project(XWritable        &C1,
                                  const Level       L1,
                                  const XReadable  &C0,
                                  const Level       L0)
        {
            for(const SNode *sn = rcl.species.head;sn;sn=sn->next)
            {
                const size_t * const indx = (**sn).indx;
                Cin[ indx[SubLevel] ] = C1[ indx[L1] ] - C0[ indx[L0] ];
            }

            XAdd &       xadd = afm.xadd;
            const size_t m    = nsp;

            xadd.make(m);
            for(size_t j=m;j>0;--j)
            {
                assert(xadd.isEmpty());
                for(size_t k=m;k>0;--k)
                {
                    const xreal_t p = pro[j][k] * Cin[k];
                    xadd << p;
                }
                Cex[j] = xadd.sum()/psf;
            }


            for(const SNode *sn = rcl.species.head;sn;sn=sn->next)
            {
                const size_t * const indx = (**sn).indx;
                C1[ indx[L1] ] = Cex[indx[SubLevel]] + C0[ indx[L0] ];
            }

        }

        void Normalizer:: setProj( )
        {
            const size_t n = rcl.Nu.rows;
            const size_t m = nsp;
            const Matrix<int> &Nu = rcl.Nu;
            Matrix<apq> Nu2(n,n);
            for(size_t i=1;i<=n;++i)
            {
                for(size_t j=i;j<=n;++j)
                {
                    apq sum = 0;
                    for(size_t k=1;k<=m;++k)
                    {
                        sum += Nu[i][k] * Nu[j][k];
                    }
                    Nu2[i][j] = Nu2[j][i] = sum;
                }
            }

            MKL::LU<apq> lu(n);
            Matrix<apq>  aNu2(Nu2); if(!lu.build(aNu2)) throw Specific::Exception(CallSign,"corrupted topology");
            apz          dNu2 = lu.determinant(aNu2).numer; assert(dNu2!=0);
            lu.adjoint(aNu2,Nu2);
            Matrix<apz> aNu3(n,m);
            {
                for(size_t i=1;i<=n;++i)
                {
                    for(size_t j=1;j<=m;++j)
                    {
                        apz sum  = 0;
                        for(size_t k=1;k<=n;++k)
                        {
                            sum += aNu2[i][k].numer * Nu[k][j];
                        }
                        aNu3[i][j] = sum;
                    }
                }
            }

            Matrix<apz> P(m,m);
            {
                apn g = 0;
                for(size_t i=1;i<=m;++i)
                {
                    for(size_t j=i;j<=m;++j)
                    {
                        apz sum = 0;
                        for(size_t k=1;k<=n;++k)
                        {
                            sum += Nu[k][i] * aNu3[k][j];
                        }
                        P[i][j] = P[j][i] = sum;
                        {
                            const apn gtmp = apn::GCD(sum.n,dNu2.n);
                            if(g<=0)
                                g = gtmp;
                            else
                                g = Min(g,gtmp);
                        }
                    }
                }

                if(g>1)
                {
                    Coerce(dNu2.n) /= g;
                    for(size_t i=1;i<=m;++i)
                    {
                        for(size_t j=1;j<=m;++j)
                        {
                            Coerce(P[i][j].n) /= g;
                        }
                    }
                    std::cerr << "P    = " << P << std::endl;
                    std::cerr << "dNu2 = " << dNu2 << std::endl;
                }
            }

            for(size_t i=1;i<=m;++i)
                for(size_t j=1;j<=m;++j)
                    Coerce(pro[i][j]) = P[i][j].cast<long>("projection matrix");
            Coerce(psf) = dNu2.cast<long>("scaling factor");
            std::cerr << "psf=" << psf << std::endl;
            std::cerr << "pro=" << pro << std::endl;

        }


        
      
    }

}

