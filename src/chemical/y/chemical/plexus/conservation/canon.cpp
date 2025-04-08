
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

                {
                    CxxListOf<QMat> qmat;
                    const size_t    n = size;
                    const size_t    k = rank;
                    Combination comb(n,k);
                    std::cerr << "#matrices(" << n << "," << k << ")=" << comb.total << std::endl;
                    Matrix<apq>  AA(k,k);
                    XAdd         xadd;
                    do
                    {
                        const Readable<size_t> &sched = comb;
                        for(size_t i=1;i<=k;++i)
                        {
                            const size_t I = sched[i];
                            const Readable<unsigned> &aI = uAlpha[I];
                            for(size_t j=1;j<=k;++j)
                            {
                                const size_t J = sched[j];
                                const Readable<unsigned> &aJ = uAlpha[J];
                                apn sum = 0;
                                for(size_t l=species->size;l>0;--l)
                                {
                                    const apn x = aI[l];
                                    const apn y = aJ[l];
                                    sum += x*y;
                                }
                                AA[i][j] = sum;
                            }
                        }
                        const size_t ar = MKL::Rank::Of(AA);
                        if(k!=ar) continue;

                        bool found = false;
                        for(const QMat *lhs=qmat.head;lhs;lhs=lhs->next)
                        {
                            if(*lhs==AA)
                            {
                                found = true;
                                break;
                            }
                        }
                        if(found)
                        {
                            std::cerr << "[multiple]" << std::endl;
                        }
                        else
                        {
                            qmat.pushTail( new QMat(AA) );
                            std::cerr << "AA" << qmat.size << "=" << AA << std::endl;
                        }

                    } while(comb.next());
                    Y_XML_COMMENT(xml, "matrices: " << qmat.size << " / " << comb.total);

                }

            }

        }

    }

}
