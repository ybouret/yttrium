
#include "y/chemical/plexus/initial/design.hpp"
#include "y/system/exception.hpp"
#include "y/mkl/algebra/lu.hpp"
#include "y/text/plural.hpp"
#include "y/mkl/algebra/rank.hpp"
#include "y/apex/api/simplify.hpp"
#include "y/apex/api/count-non-zero.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Initial
        {

            const char * const Design:: CallSign            = "Initial::Design";
            const char * const Design:: Logo                = "Logo";
            const char * const Design:: LogoExpr            = "[.][:word:]+";
            const char * const Design:: _FixedConcentration = "FixedConcentration";
            const char * const Design:: _SteadyConservation = "SteadyConservation";

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


            class Prospect : public Quantized
            {
            public:
                typedef CxxArray<apz,Memory::Dyadic> zArray;
                typedef CxxArray<apq,Memory::Dyadic> qArray;
                typedef CxxListOf<Prospect>          List;

                explicit Prospect(const Readable<apq> &q,
                                  const Readable<apq> &p) :
                Q(q.size()),
                P(CopyOf,p),
                next(0),
                prev(0)
                {
                    qArray    tmp(CopyOf,q);
                    const apn fac = Apex::Simplify::Array(tmp);
                    Q.ld(tmp);
                    for(size_t i=P.size();i>0;--i) P[i] *= fac;
                    //std::cerr << "q=" << q << " -> " << Q << " / " << fac << std::endl;
                    //std::cerr << "p=" << p << " -> " << P << std::endl;
                }

                virtual ~Prospect() noexcept {}

                template <typename RHS> inline
                std::ostream & display(std::ostream &os, RHS &rhs ) const
                {
                    os << Q << "*delta=" << P << "'*" << rhs;
                    return os;
                }


                zArray    Q;
                qArray    P;
                Prospect *next;
                Prospect *prev;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Prospect);
            };

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

                //--------------------------------------------------------------
                //
                //
                // Computing Projection and Orthogonal Matrices
                //
                //
                //--------------------------------------------------------------
                const size_t     Nq = M-Np;
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
                std::cerr << "Np=" << Np << std::endl;
                std::cerr << "Nq=" << Nq << std::endl;

                lib.show(std::cerr << "Cs=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;



                {
                    Prospect::List pros;
                    for(Library::ConstIterator it=lib->begin();it!=lib->end();++it)
                    {
                        const Species &sp = **it;
                        const size_t   j  = sp.indx[TopLevel];
                        const Readable<apq> &Qj = Q[j]; if(Apex::CountNonZero(Qj)<=0) continue;
                        Prospect &pro = *pros.pushTail( new Prospect(Qj,P[j]) );

                        pro.display( lib.pad(std::cerr << sp.name,sp) << " : ",b) << std::endl;
                    }
                }

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
