
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
            uAlpha(),
            xAlpha(),
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
#include "y/sequence/vector.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {
            using namespace MKL;

#if 0
            class QMat : public Matrix<apq>
            {
            public:
                explicit QMat(const Matrix<apq> &M) :
                Matrix<apq>(M),
                next(0),
                prev(0)
                {
                }

                virtual ~QMat() noexcept {}

                QMat *next;
                QMat *prev;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(QMat);
            };


            class QMatList : public CxxListOf<QMat>
            {
            public:
                explicit QMatList() noexcept : CxxListOf<QMat>() {}
                virtual ~QMatList() noexcept {}

                bool has(const Matrix<apq> &M) const noexcept
                {
                    for(const QMat *mine=head;mine;mine=mine->next)
                    {
                        if( *mine == M ) return true;
                    }
                    return false;
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(QMatList);
            };


            static inline apq Dot(const Readable<unsigned> &lhs,
                                  const Readable<unsigned> &rhs)
            {
                assert(lhs.size()==rhs.size());
                apn res = 0;
                for(size_t i=lhs.size();i>0l;--i)
                {
                    const apn l = lhs[i];
                    const apn r = rhs[i];
                    res += l*r;
                }
                return res;
            }
#endif
            
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
                        const Law &law = **l;
                        enroll( law );
                        Coerce(law.auxId) = ++auxId;
                        Y_XMLOG(xml,"(+) " <<law.name);
                        for(const Actor *a=law->head;a;a=a->next)
                        {
                            Coerce(species) << a->sp;
                            Coerce(sformat).enroll(a->sp);
                        }
                    }
                }



                Y_XML_COMMENT(xml, std::setw(4) << species->size << " = # " << species);

                //--------------------------------------------------------------
                //
                //
                // compute the rank of the local matrices
                //
                //
                //--------------------------------------------------------------
                {
                    uMatrix    &uA = Coerce(uAlpha);
                    XMatrix    &xA = Coerce(xAlpha);
                    uA.make(size,species->size);
                    xA.make(size,species->size);
                    for(const LNode *ln=head;ln;ln=ln->next)
                    {
                        const Law &law = **ln;
                        law.iFillArray(uA[law.auxId], AuxLevel);
                        law.xFillArray(xA[law.auxId], AuxLevel);
                    }
                    //Y_XMLOG(xml, "A   =" << iA);
                    Coerce(rank) = Rank::Of(uA);
                    //Y_XMLOG(xml, "rank=" << rank);
                    Y_XML_COMMENT(xml, std::setw(4) << rank << " = rank");
                }

                //--------------------------------------------------------------
                //
                //
                // collect concerned definite into anxious
                //
                //
                //--------------------------------------------------------------
                for(const ENode *en=definite.head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    if(eq.gotAnyOf(*species))
                    {
                        Coerce(anxious) << eq;
                        Y_XMLOG(xml, "(*) " << eq);
                    }
                }
                Y_XML_COMMENT(xml, std::setw(4) << anxious->size <<  " = #anxious");

#if 0
                {
                    QMatList        qmat;
                    const size_t    n = size;
                    const size_t    k = rank;
                    const size_t    m = species->size;
                    Combination comb(n,k);
                    std::cerr << "#matrices(" << n << "," << k << ")=" << comb.total << std::endl;

                    Matrix<unsigned> A(k,m);
                    Matrix<apq>      AA(k,k);
                    Matrix<apq>      II(k,k);
                    MKL::LU<apq>     lu(k);
                    Matrix<apq>      AT(m,k);
                    //Matrix<unsigned> AT(m,k);
                    do
                    {
                        // load basis
                        for(size_t i=k;i>0;--i)
                            A[i].ld( uAlpha[ comb[i] ]);

                        // gram
                        for(size_t i=k;i>0;--i)
                        {
                            const Readable<unsigned> &Ai = A[i];
                            for(size_t j=i;j>0;--j)
                            {
                                const Readable<unsigned> &Aj= A[j];
                                if(i!=j)
                                    AA[j][i] = AA[i][j] = Dot(Ai,Aj);
                                else
                                    AA[i][i] = Dot(Ai,Aj);
                            }
                        }
                        //std::cerr << "AA=" << AA << std::endl;
                        if(!lu.build(AA)) continue;
                        lu.invert(AA,II);
                        //std::cerr << "II=" << II << std::endl;
                        if(qmat.has(II)) continue;
                        qmat.pushTail( new QMat(AA) );
                        AT.assign(TransposeOf,A);

                        (std::cerr << comb << " #" << qmat.size << "     \r").flush();


                    }
                    while(comb.next());
                    std::cerr << std::endl;
                    Y_XML_COMMENT(xml, "matrices: " << qmat.size << " / " << comb.total);
                }
#endif
            }

        }

    }

}
