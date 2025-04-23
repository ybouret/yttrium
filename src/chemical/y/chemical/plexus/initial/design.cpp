
#include "y/chemical/plexus/initial/design.hpp"
#include "y/system/exception.hpp"
#include "y/mkl/algebra/lu.hpp"
#include "y/text/plural.hpp"
#include "y/mkl/algebra/ortho-space.hpp"
#include "y/apex/api/ortho/coven/survey/api.hpp"
#include "y/apex/api/ortho/architect.hpp"
#include "y/mkl/algebra/rank.hpp"

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

                //--------------------------------------------------------------
                //
                //
                // initialize
                //
                //
                //--------------------------------------------------------------
                lib.ldz(C0);
                const size_t M  = lib->size();
                const size_t Np = my.size;
                Y_XML_SECTION_OPT(xml,
                                  CallSign,
                                  "species=" << M
                                  << " axiom" << Plural::s(Np) << "=" << Np);

                //--------------------------------------------------------------
                //
                //
                // check status
                //
                //
                //--------------------------------------------------------------
                if(Np<=0)
                {
                    Y_XML_COMMENT(xml, "no axiom");
                    return;
                }

                if(Np>M)
                {
                    throw Specific::Exception(CallSign, "overdetermined design");
                }

                const size_t Nq = M-Np;
                Matrix<apq>      P(M,Np);
                Matrix<xreal_t>  P_(M,Np);
                Matrix<apq>      Q(M,M);
                Matrix<apq>      B(Np,M);
                XArray           b(Np);   fillDesignMatrix(B, b, my, lib);
                Matrix<apq>      BT(TransposeOf,B);
                Matrix<apq>      iB2(Np,Np);
                {
                    // B2 = B*BT
                    Matrix<apq> B2(Np,Np);
                    for(size_t i=1;i<=Np;++i)
                    {
                        for(size_t j=1;j<=i;++j)
                            B2[i][j] = B2[j][i] = Dot(B[i],B[j]);
                    }
                    {
                        MKL::LU<apq> lu(Np);
                        if(!lu.build(B2)) throw Specific::Exception(CallSign,"singular axiom%s",Plural::s(Np));
                        lu.invert(B2,iB2);
                    }
                }

                std::cerr << "B=" << B << std::endl;
                std::cerr << "b=" << b << std::endl;
                for(size_t i=1;i<=M;++i)
                {
                    for(size_t j=1;j<=Np;++j)
                    {
                        P_[i][j] = (P[i][j] = Dot(BT[i],iB2[j])).to<real_t>();
                    }
                }

                {
                    const apq _1 = 1;
                    for(size_t i=1;i<=M;++i)
                    {
                        for(size_t j=1;j<i;++j)
                            Q[i][j] =    - Dot(P[i],BT[j]);
                        Q[i][i]     = _1 - Dot(P[i],BT[i]);
                        for(size_t j=i+1;j<=M;++j)
                            Q[i][j] =    - Dot(P[i],BT[j]);
                    }
                }
                assert(Np==MKL::Rank::Of(P));
                assert(Nq==MKL::Rank::Of(Q));

                std::cerr << "P=" << P << std::endl;
                std::cerr << "Q=" << Q << std::endl;

                XAdd xadd;
                for(Library::ConstIterator it=lib->begin();it!=lib->end();++it)
                {
                    const Species &sp = **it;
                    sp(C0,TopLevel) = xadd.dot( sp(P_,TopLevel), b);
                }

                std::cerr << "Cs=" << C0 << std::endl;
                lib.show(std::cerr << "Cs=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;


#if 0
                //--------------------------------------------------------------
                //
                //
                // initialize
                //
                //
                //--------------------------------------------------------------
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

                //--------------------------------------------------------------
                //
                //
                // check status
                //
                //
                //--------------------------------------------------------------
                if(info>M)
                    throw Specific::Exception(CallSign,"too much information");

                if(Nc<=0) {
                    Y_XML_COMMENT(xml, "no axiom");
                    return;
                }


                Matrix<apq> P(Nc,M);
                XArray      p(Nc);   fillDesignMatrix(P, p, my, lib);
                Matrix<apq> PT(TransposeOf,P);
                Matrix<apq> iPP(Nc,Nc);
                {
                    Matrix<apq> PP(Nc,Nc);
                    for(size_t i=1;i<=Nc;++i)
                    {
                        for(size_t j=1;j<=i;++j)
                            PP[i][j] = PP[j][i] = Dot(P[i],P[j]);
                    }
                    std::cerr << "P=" << P << std::endl;
                    std::cerr << "p=" << p << std::endl;
                    std::cerr << "PP=" << PP << std::endl;
                    {
                        MKL::LU<apq> lu(Nc);
                        if(!lu.build(PP)) throw Specific::Exception(CallSign,"singular axiom%s",Plural::s(Nc));
                        lu.invert(PP,iPP);
                    }
                    std::cerr << "iPP=" << iPP << std::endl;
                }
                std::cerr << "// rank(P)=" << MKL::Rank::Of(P) << " / " << Nc << std::endl;
                Matrix<xreal_t> P3(M,Nc);
                for(size_t i=1;i<=M;++i)
                {
                    for(size_t j=1;j<=Nc;++j)
                    {
                        const apq q = Dot(PT[i],iPP[j]);
                        P3[i][j] = q.to<real_t>();
                    }
                }
                std::cerr << "P3=" << P3 << std::endl;

                XAdd xadd;
                for(Library::ConstIterator it=lib->begin();it!=lib->end();++it)
                {
                    const Species &sp = **it;
                    sp(C0,TopLevel) = xadd.dot( sp(P3,TopLevel), p);
                }




#if 1
                if(info<M)
                {
                    const size_t dof = M-info;
                    Matrix<apz>  iQ;
                    if(!MKL::OrthoSpace::Make(iQ,P))
                    {
                        throw Specific::Exception(CallSign,"couldn't make OrthoSpace");
                    }
                    std::cerr << "Q=" << iQ << std::endl;
                    std::cerr << "dof=" << dof << std::endl;

                    for(size_t i=1;i<=M;++i)
                    {
                        
                    }

                    Matrix<apz> Q;



                    if(false)
                    {
                        Coerce(xml.verbose) = false;
                        //if(false)
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
#endif
                std::cerr << "Cs=" << C0 << std::endl;
                lib.show(std::cerr << "Cs=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;


#endif

            }


        }

    }

}


#include "y/lingo/pattern/matching.hpp"
#include "y/chemical/plexus/initial/axiom/steady-conservation.hpp"
namespace Yttrium
{
    namespace Chemical
    {
        namespace Initial
        {

            void Design:: add(Clusters &     clusters,
                              const String & expr,
                              const xreal_t  Ctot)
            {
                Lingo::Matching                 m(expr,0);
                const Conservation::Law * const law = clusters.preserving(m);
                if(0==law) throw Specific::Exception(CallSign, "no conservation matching '%s' in %s", expr.c_str(), Clusters::CallSign);
                my.pushTail( new SteadyConservation(*law,Ctot) );
            }

        }

    }
}
