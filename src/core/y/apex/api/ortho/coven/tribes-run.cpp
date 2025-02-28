
#include "y/apex/api/ortho/coven/tribes.hpp"

namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {
            namespace Coven
            {
                static inline
                void promote(Posture       &posture,
                             const Content &foreign) noexcept
                {
                    ListOf<INode> conserved;
                    while(posture.residue.size>0)
                    {
                        INode *node = posture.residue.popHead();
                        if(foreign->has(**node))
                        {
                            posture.content.push(node);
                        }
                        else
                        {
                            conserved.pushTail(node);
                        }
                    }
                    posture.residue.swapWith(conserved);
                }

                static inline
                void collapse(Posture &lhs, Posture &rhs)
                {
                    Posture lhsNew = lhs; promote(lhsNew,rhs.content);
                    Posture rhsNew = rhs; promote(rhsNew,lhs.content);
                    lhs.xch(lhsNew);
                    rhs.xch(rhsNew);
                }

                void Tribes:: process(XMLog &xml, const unsigned flag)
                {
                    Y_XML_COMMENT(xml, "#generated   = " << size);
                    Y_XML_COMMENT(xml, "#collected   = " << collected);

                    if( 0 != (flag&RemoveFutile) )
                    // first pass: remove futile
                    {
                        Tribe::List active;
                        while(size>0)
                        {
                            Tribe * const tribe = popHead();
                            if(Foundation == tribe->qfamily->quality || tribe->residue.size<=0)
                            {
                                delete tribe;
                                continue;
                            }
                            active.pushTail(tribe);
                        }
                        swapWith(active);
                        assert(isSortedAccordingTo(Tribe::Compare));
                    }

                    // second pass: remove alike
                    if( 0 != (flag&FindMultiple) )
                    {
                        Tribe::List kept;
                        size_t      drop = 0;
                        while(size>0)
                        {
                            AutoPtr<Tribe> lhs = popHead();
                            Posture       &L = *lhs;
                            for(Tribe *rhs=kept.head;rhs;rhs=rhs->next)
                            {
                                Posture &R = *rhs;
                                if(L==R)
                                {
                                    ++drop;
                                    continue;
                                }
                            }
                            kept.pushTail( lhs.yield() );
                        }
                        swapWith(kept);
                        assert(isSortedAccordingTo(Tribe::Compare));
                        Y_XML_COMMENT(xml, "#drop-same   = " << drop);
                    }




                    if(size<=0)
                        MergeSort::Call( Coerce(db), CompareVectors);

                }


            }

        }

    }

}


