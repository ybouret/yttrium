
#include "y/chemical/plexus/conservation/warden.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Conservation
        {

            const char * const Warden:: CallSign = "Conservation::Warden";

            Warden:: ~Warden() noexcept {}

            Warden:: Warden(const Cluster &_cluster,
                            const Canon   &_canon) :
            cluster(_cluster),
            canon(_canon),
            n(canon.rank),
            m(canon.species->size),
            bbank(),
            blist(bbank),
            basis(bbank),
            xadd(),
            metrics( canon.species->size ),
            qvcache( new QVector::Cache(metrics) ),
            qfamily(qvcache),
            lu(canon.rank),
            A(n,m),
            AT(m,n),
            AA(n,n),
            Xs(n)
            {
                assert(cluster.canons.owns(&canon));
                bbank->reserve(canon.size+canon.rank);
                qvcache->reserve(canon.rank);
            }

            static inline
            SignType CompareBroken(const BNode * const lhs,
                                   const BNode * const rhs) noexcept
            {
                const xreal_t zero;
                const Broken &L = **lhs;
                const Broken &R = **rhs;
                const xreal_t lxs = L.xs;
                const xreal_t rxs = R.xs;

                if(lxs<=zero)
                {
                    if(rxs<=zero)
                    {
                        // then ordered by index
                        return Sign::Of(L.law.auxId,R.law.auxId);
                    }
                    else
                    {
                        assert(lxs<rxs);
                        return Positive;
                    }
                }
                else
                {
                    // lxs >  0
                    if(rxs<=zero)
                    {
                        assert(rxs<lxs);
                        return Negative;
                    }
                    else
                    {
                        // decreasing comparison
                        return Sign::Of(rxs,lxs);
                    }
                }

            }


            void Warden:: fix(XMLog     &  xml,
                              XWritable &  C0,
                              Summator  &  I0,
                              const Level  L0,
                              AddressBook &vanishing)
            {
                Y_XML_SECTION(xml,CallSign);
                const xreal_t zero;

                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                blist.free();
                basis.free();
                qfamily.reset();

                //--------------------------------------------------------------
                //
                // probe
                //
                //--------------------------------------------------------------
                size_t numBroken = 0;
                for(const LNode *ln=canon.head;ln;ln=ln->next)
                {
                    const Law &   law = **ln;
                    const xreal_t lxs = law.excess(xadd, C0, L0);
                    if(lxs>zero)
                    {
                        ++numBroken;
                        law.sendTo(vanishing);
                    }
                    const Broken broken(lxs,law);
                    blist << broken;
                }
                assert(canon.size==blist.size);

                //--------------------------------------------------------------
                //
                // sort
                //
                //--------------------------------------------------------------
                MergeSort::Call(blist,CompareBroken);
                Y_XML_COMMENT(xml, "broken " << numBroken << " / " << canon.size );
                if(xml.verbose)
                {
                    for(const BNode *bn=blist.head;bn;bn=bn->next)
                    {
                        const Broken &     broken = **bn;
                        const char * const prefix = (broken.xs<=zero) ? "(+)" : "(-)";
                        (**bn).show( xml() << prefix << ' ',canon) << std::endl;
                    }
                }
                if(numBroken<=0) return;


                //--------------------------------------------------------------
                //
                // extract basis with rank of canon
                //
                //--------------------------------------------------------------
                Y_XML_COMMENT(xml,"extract basis");
                for(const BNode *bn  = blist.head;bn;bn=bn->next)
                {
                    const Broken &broken = **bn;
                    const Law    &law    = broken.law;
                    if(qfamily.welcomes( canon.iAlpha[law.auxId]) )
                    {
                        if(xml.verbose) broken.show( xml() << "(*) ",canon) << std::endl;
                        qfamily.increase();
                        basis << broken;
                        if(qfamily->size>=canon.rank) goto FOUND_BASIS;
                    }
                }

            FOUND_BASIS:
                assert(basis.size==canon.rank);

                //--------------------------------------------------------------
                //
                // construct algebra
                //
                //--------------------------------------------------------------
                {
                    size_t i=1;
                    for(const BNode *lhs=basis.head;lhs;lhs=lhs->next,++i)
                    {
                        const Broken     & L       = **lhs;
                        XMatrix::RowType & AA_i    = AA[i];
                        const XReadable  & alpha_i = canon.xAlpha[ L.law.auxId];
                        A[i].ld(alpha_i);
                        Xs[i] = L.xs;
                        {
                            size_t j=i;
                            for(const BNode *rhs=lhs;rhs;rhs=rhs->next,++j)
                            {
                                const Broken     & R       = **rhs;
                                const XReadable  & alpha_j = canon.xAlpha[ R.law.auxId];
                                AA[j][i] = AA_i[j] = xadd.dot(alpha_i,alpha_j);
                            }
                        }
                    }
                }
                AT.assign(TransposeOf,A);

#if 0
                std::cerr << "A =" << A  << std::endl;
                std::cerr << "AA=" << AA << std::endl;
                std::cerr << "Xs="  << Xs << std::endl;
                std::cerr << "dC=A'*inv(AA)*Xs" << std::endl;
#endif

                if(!lu.build(AA)) throw Specific::Exception(CallSign,"Corrupted constraint basis!!");

                //--------------------------------------------------------------
                //
                // increase concentrations
                //
                //--------------------------------------------------------------
                lu.solve(AA,Xs);
                for(const SNode *sn=canon.species->head;sn;sn=sn->next)
                {
                    const Species   &sp  = **sn;
                    const size_t     j   = sp.indx[AuxLevel];
                    const XReadable &lhs = AT[j];
                    xreal_t         &Cj  = sp(C0,L0);
                    const xreal_t    c0  = Cj;
                    xadd.free();
                    xadd << Cj;
                    for(size_t i=n;i>0;--i) xadd << (lhs[i]*Xs[i]);
                    const xreal_t c1 = Cj = xadd.sum();
                    const xreal_t delta = c1-c0;
                    sp(I0,L0) << delta;
                    if(xml.verbose) {
                        cluster->sformat.pad( xml() << "d[" << sp.name << "]",sp) << " = " << delta.str() << std::endl;
                    }
                }
                
                for(const LNode *ln=canon.head;ln;ln=ln->next)
                {
                    const Law &   law = **ln;
                    const xreal_t xs  = law.excess(xadd,C0,L0);
                    canon.pad(std::cerr << law.name, law) << " @" << xs.str() << std::endl;
                }


            }
        }
    }

}


