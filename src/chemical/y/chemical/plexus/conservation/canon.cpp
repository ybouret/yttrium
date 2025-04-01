
#include "y/chemical/plexus/conservation/canon.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {


            Canon:: Canon(const Law &first) :
            LList(),
            rank(0),
            species(),
            anxious(),
            sformat(),
            next(0),
            prev(0)
            {
                (*this) << first;
            }

            Canon:: ~Canon() noexcept
            {
            }

            bool Canon:: accepts(const Law &law) const noexcept
            {
                for(const LNode *ln=head;ln;ln=ln->next)
                {
                    if( (**ln).hasCommonActorWith(law) ) return true;
                }
                return false;
            }


            bool Canon:: accepts(const Canon &rhs) const noexcept
            {
                for(const LNode *l=head;l;l=l->next)
                {
                    if(rhs.accepts(**l)) return true;
                }
                return false;
            }

        }

    }

}

#include "y/text/plural.hpp"
#include "y/mkl/algebra/rank.hpp"
#include "y/mkl/algebra/lu.hpp"
#include "y/counting/combination.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {
            using namespace MKL;

            void Canon:: compile(XMLog       & xml,
                                 const EList & definite)
            {
                Y_XML_SECTION_OPT(xml, "Conservation::Canon", "size=" << size);
                assert( 0 == maxNameLength );
                assert( 0 == species->size );

                //--------------------------------------------------------------
                //
                //
                // collect species and make AuxLevel
                //
                //
                //--------------------------------------------------------------
                Y_XML_COMMENT(xml, "law" << Plural::s(size));
                {
                    size_t auxId = 0;
                    for(const LNode *l=head;l;l=l->next)
                    {
                        const Law &law = **l; enroll( law );
                        Coerce(law.auxId) = ++auxId;
                        Y_XMLOG(xml,"(+) " <<law.name);
                        for(const Actor *a=law->head;a;a=a->next)
                        {
                            Coerce(species) << a->sp;
                            Coerce(sformat).enroll(a->sp);
                        }
                    }
                }



                Y_XML_COMMENT(xml, "species");;
                Y_XMLOG(xml, "(#) " << species);

                //--------------------------------------------------------------
                //
                //
                // compute the rank of the local matrix
                //
                //
                //--------------------------------------------------------------
                {
                    Y_XML_COMMENT(xml, "rank");
                    Matrix<apq> Q(size,species->size);
                    {
                        size_t i=1;
                        for(const LNode *ln=head;ln;ln=ln->next,++i)
                        {
                            const Law &law = **ln;
                            law.fillArray(Q[i],AuxLevel);
                        }
                    }
                    Y_XMLOG(xml, "Q   =" << Q);
                    Coerce(rank) = Rank::Compute(Q);
                    Y_XMLOG(xml, "rank=" << rank);
                }

                //--------------------------------------------------------------
                //
                //
                // compute all possibilities
                //
                //
                //--------------------------------------------------------------
                LU<apq> lu(rank);
                const size_t n = size;
                const size_t m = species->size;
                const apq    q0;

                for(size_t k=1;k<=rank;++k)
                {
                    Matrix<apz> A(k,m);
                    Matrix<apq> A2(k,k);
                    Matrix<apq> Q2(k,k); // to test rank
                    Matrix<apq> adjA2(k,k);
                    Matrix<apz> A3(k,m); // adjA2 * A
                    Matrix<apz> A4(m,m); //! A' * A3

                    size_t      nmat = 0;
                    Combination comb(n,k);
                    Y_XML_SECTION_OPT(xml,"Projection", " rank=" << k << " maxi=" << comb.total);

                    do
                    {
                        A.ld(q0);
                        for(size_t i=1;i<=k;++i)
                        {
                            const Law &law = **fetch( comb[i] );
                            law.fillArray(A[i],AuxLevel);
                        }
                        
                        for(size_t i=k;i>0;--i)
                        {
                            const Readable<apz> &Ai = A[i];
                            for(size_t j=i;j>0;--j)
                            {
                                const Readable<apz> &Aj = A[j];
                                apz sum = 0; for(size_t l=m;l>0;--l) sum += Ai[l] * Aj[l];
                                A2[i][j] = A2[j][i] = sum;
                            }
                        }

                        Q2.assign(A2); if(k!=Rank::Compute(Q2)) continue;
                        std::cerr << "A="  << A  << std::endl;
                        std::cerr << "A2=" << A2 << std::endl;
                        lu.adjoint(adjA2,A2);
                        if(!lu.build(A2)) throw Specific::Exception("Chemical::Canon","corrupted Gram Matrix!!");
                        const apz detA2 = lu.determinant(A2);
                        std::cerr << "adjA2=" << adjA2 << std::endl;
                        std::cerr << "detA2=" << detA2 << std::endl;

                        for(size_t i=k;i>0;--i)
                        {
                            for(size_t j=m;j>0;--j)
                            {
                                apz sum = 0;
                                for(size_t l=k;l>0;--l)
                                {
                                    sum += adjA2[i][l].numer * A[l][j];
                                }
                                A3[i][j] = sum;
                            }
                        }
                        std::cerr << "A3=" << A3 << std::endl;

                        for(size_t i=m;i>0;--i)
                        {
                            for(size_t j=i;j>0;--j)
                            {
                                apz sum = 0;
                                for(size_t l=k;l>0;--l)
                                {
                                    sum -= A[l][i] * A3[l][j];
                                }
                                if(i==j) sum += detA2;
                                A4[i][j] = A4[j][i] = sum;
                            }
                        }
                        std::cerr << "A4=" << A4 << std::endl;


                        ++nmat;
                    } while( comb.next() );
                    Y_XML_COMMENT(xml, "found " << nmat << " / " << comb.total);




                }



                //--------------------------------------------------------------
                //
                //
                // collect definite into anxious
                //
                //
                //--------------------------------------------------------------
                Y_XML_COMMENT(xml, "anxious");
                for(const ENode *en=definite.head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    if(eq.gotAnyOf(*species))
                    {
                        Coerce(anxious) << eq;
                        Y_XMLOG(xml, "(*) " << eq);
                    }
                }

            }

        }

    }

}
