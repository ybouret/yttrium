
#include "y/chemical/plexus/initial/design.hpp"
#include "y/system/exception.hpp"
#include "y/mkl/algebra/lu.hpp"
#include "y/text/plural.hpp"
#include "y/mkl/algebra/rank.hpp"
#include "y/apex/api/simplify.hpp"
#include "y/apex/api/count-non-zero.hpp"
#include "y/apex/api/ortho/architect.hpp"
#include "y/chemical/plexus/initial/axiom/fixed-concentration.hpp"
#include "y/apex/api/ortho/coven/survey/natural.hpp"
#include "y/apex/api/ortho/coven/survey/integer.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Initial
        {

            bool Design:: foundZeroConcentrationIn(XMLog &      xml,
                                                   const SList &list)
            {
                for(const SNode *sn=list.head;sn;sn=sn->next)
                {

                    // check if species is explicitly defined
                    const Species &sp = **sn; if(defines(sp)) continue;

                    // if not, set it to zero
                    my.pushTail( new FixedConcentration(sp,0) );
                    Y_XML_COMMENT(xml, "setting [" << sp << "]=0");
                    assert(defines(sp));
                    return true;
                }
                return false;
            }


            bool Design:: foundZeroConcentration(XMLog          &xml,
                                                 const Clusters &cls)
            {
                assert(latched);

                if( foundZeroConcentrationIn(xml, cls.conserved.charged) ) return true;
                if( foundZeroConcentrationIn(xml, cls.spectator.charged) ) return true;

                return false;
            }


            static  inline apq Dot(const Readable<apq> &a, const Readable<apq> &b)
            {
                assert(a.size()==b.size());
                apq res = 0;
                for(size_t i=a.size();i>0;--i)
                    res += a[i] * b[i];
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
                                const Library  &lib,
                                const Clusters &cls)
            {
                if(!latched) throw Specific::Exception(CallSign,"building only if latched!");

                //assert(Cs.size()>=lib->size());

                //--------------------------------------------------------------
                //
                //
                // initialize
                //
                //
                //--------------------------------------------------------------
                const size_t M  = lib->size();
                const size_t N  = cls.primary;
                size_t       Np = my.size;
                Y_XML_SECTION_OPT(xml,
                                  CallSign,
                                  "species=" << M
                                  << " equilibri" << Plural::aum(N) << "=" << N
                                  << " axiom"     << Plural::s(Np)  << "=" << Np);


                if(Np<=0)
                {
                    Y_XML_COMMENT(xml,"no axioms");
                    return;
                }


                //--------------------------------------------------------------
                //
                //
                // sorting out
                //
                //
                //--------------------------------------------------------------
                const size_t Nc = N + Np;
                Y_XML_COMMENT(xml, Nc << " constraint" <<  Plural::s(Nc) << " for " << M << " species");
                if(Nc>M)
                {
                    throw Specific::Exception(CallSign,"too many axioms");
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


                XAdd xadd;
                XArray Cs(M,0);
                for(Library::ConstIterator it=lib->begin();it!=lib->end();++it)
                {
                    const Species &sp = **it;
                    sp(Cs,TopLevel) = xadd.dot( sp(P_,TopLevel), b);
                }
                

                std::cerr << "P="  << P  << std::endl;
                std::cerr << "Q="  << Q  << std::endl;
                std::cerr << "Cs=" << Cs << std::endl;
                std::cerr << "Np=" << Np << std::endl;
                std::cerr << "Nq=" << Nq << std::endl;
                //std::cerr << "Nu=" << Nu << std::endl;
                //std::cerr << "Mu=" << Mu << std::endl;

                lib.show(std::cerr << "Cs=", "\t[", Cs, "]", xreal_t::ToString ) << std::endl;




            }

        }

    }

}
