
#include "y/chemical/plexus/initial/design.hpp"
#include "y/system/exception.hpp"
#include "y/mkl/algebra/lu.hpp"
#include "y/text/plural.hpp"
#include "y/mkl/algebra/ortho-space.hpp"
#include "y/apex/api/ortho/coven/survey/api.hpp"
#include "y/apex/api/ortho/architect.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Initial
        {

            const char * const Design:: CallSign = "Initial::Design";

            Y_PROXY_IMPL(Design,my)


            Design:: ~Design() noexcept
            {

            }

            void Design:: add(Axiom * const axiom) noexcept
            {
                assert(0!=axiom);
                assert(!my.owns(axiom));
                my.pushTail(axiom);
            }

            
            static  inline apq Dot(const Readable<apq> &a, const Readable<apq> &b)
            {
                assert(a.size()==b.size());
                apq res = 0;
                for(size_t i=a.size();i>0;--i)
                {
                    res += a[i] * b[i];
                }
                return res;
            }


            template <typename MATRIX>
            static inline void fillDesignMatrix(MATRIX              &P,
                                                XWritable           &p,
                                                const ListOf<Axiom> &axm,
                                                const Library       &lib)
            {
                assert(P.rows==axm.size);
                assert(P.cols==lib->size());
                size_t i = 1;
                for(const Axiom *axiom=axm.head;axiom;axiom=axiom->next,++i)
                {
                    for(Library::ConstIterator it=lib->begin();it!=lib->end();++it)
                    {
                        const Species &sp = **it;
                        sp(P[i],TopLevel) = axiom->weight(sp);
                    }
                    p[i] = axiom->amount;
                }
            }


            void Design:: build(XMLog          &xml,
                                XWritable      &C0,
                                const Library  &lib,
                                const Clusters &cls) const
            {

                lib.ldz(C0);
                const size_t N    = cls.primary;
                const size_t M    = lib->size();
                const size_t Nc   = my.size;
                const size_t info = N+Nc;

                Y_XML_SECTION_OPT(xml,
                                  CallSign,
                                  "equilibri" << Plural::aum(N) << "=" << N
                                  << " species=" << M
                                  << " axiom" << Plural::s(Nc) << "=" << Nc
                                  << " info=" << info << "/" << M
                                  );
                if(info>M) throw Specific::Exception(CallSign,"too much information");
                if(Nc<=0)
                {
                    Y_XML_COMMENT(xml, "no axiom");
                }

                Matrix<apq> P(Nc,M);
                XArray      p(Nc);
                fillDesignMatrix(P, p, my, lib);

                std::cerr << "P=" << P << std::endl;
                std::cerr << "p=" << p << std::endl;
                if(info<M)
                {
                    const size_t dof = M-info;
                    Matrix<apz> iQ;
                    if(!MKL::OrthoSpace::Make(iQ,P))
                    {
                        throw Specific::Exception(CallSign,"couldn't make OrthoSpace");
                    }
                    std::cerr << "iQ=" << iQ << std::endl;
                    std::cerr << "dof=" << dof << std::endl;
                    Matrix<apz> Q;


                    //if(false)
                    {
                        Coerce(xml.verbose) = false;
                        {
                            Apex::Ortho::Coven::AutoSurvey<apn,1> survey(xml,iQ,NULL);
                            std::cerr << survey << std::endl;
                        }

                        //if(false)
                        {
                            Apex::Ortho::Coven::AutoSurvey<apz,1> survey(xml,iQ,NULL);
                            std::cerr << survey << std::endl;
                        }
                    }
                }





            }


#if 0

                lib.ldz(C0);
                Matrix<xreal_t> B3(m,n);
                XArray          rhs(n);
                {
                    Y_XML_COMMENT(xml, "compiling " << n << " axiom" << Plural::s(n));
                    Matrix<apq>  B(n,m);
                    {
                        size_t i=1;
                        for(const Axiom *axiom=my.head;axiom;axiom=axiom->next,++i)
                        {
                            Writable<apq> &B_i = B[i];
                            for(Library::ConstIterator it=lib->begin();it!=lib->end();++it)
                            {
                                const Species &sp = **it;
                                sp(B_i,TopLevel) = axiom->weight(sp);
                            }
                            rhs[i] = axiom->amount;
                            Y_XMLOG(xml, std::setw(8) << rhs[i].str() << " @" << B_i);
                        }

                    }

                    Matrix<apz> P;
                    if(!MKL::OrthoSpace::Make(P,B))
                    {
                        throw Specific::Exception(CallSign, "no Ortho Space");
                    }

                    std::cerr << "B=" << B << std::endl;
                    std::cerr << "P=" << P << std::endl;




                    Y_XML_COMMENT(xml, "computing pseudo-inverse");
                    {
                        Matrix<apq> BB(n,n);
                        for(size_t i=1;i<=n;++i)
                        {
                            for(size_t j=1;j<=i;++j)
                            {
                                BB[i][j] = BB[j][i] = Dot(B[i],B[j]);
                            }
                        }
                        Y_XMLOG(xml, "B     = " << B);
                        Y_XMLOG(xml, "GramB = " << BB);
                        Matrix<apq> iBB(n,n);
                        {
                            MKL::LU<apq> lu(n);
                            if(!lu.build(BB))
                                throw Specific::Exception(CallSign,"singular description");
                            lu.invert(BB,iBB);
                        }
                        Y_XMLOG(xml, "iBB   = " << iBB);
                        Y_XML_COMMENT(xml, "finalizing dispatch matrix");
                        const Matrix<apq> BT(TransposeOf,B);
                        for(size_t j=1;j<=m;++j)
                        {
                            for(size_t i=1;i<=n;++i)
                            {
                                const apq q = Dot(BT[j],iBB[i]);
                                B3[j][i] = q.to<real_t>();
                            }
                        }
                    }
                }
                Y_XMLOG(xml,"mat=" << B3);
                Y_XMLOG(xml,"rhs=" << rhs);

                {
                    XAdd  xadd;
                    for(Library::ConstIterator it=lib->begin();it!=lib->end();++it)
                    {
                        const Species &sp = **it;
                        sp(C0,TopLevel) = xadd.dot(sp(B3,TopLevel),rhs);
                    }
                }

#endif


        }

    }

}
