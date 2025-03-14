
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
                bool collapse(Posture &lhs, Posture &rhs)
                {
                    {
                        Posture lhsNew = lhs; promote(lhsNew,rhs.content);
                        Posture rhsNew = rhs; promote(rhsNew,lhs.content);
                        lhs.xch(lhsNew);
                        rhs.xch(rhsNew);
                    }
                    return lhs == rhs;
                }


                Tribe *FindIndenticalFamily(const Tribe &tribe,
                                            Tribe::List &tribes) noexcept
                {
                    const Posture &L = tribe;
                    for(  Tribe *guess=tribes.head;guess;guess=guess->next)
                    {
                        const Posture &R = *guess;
                        if(L==R) continue;

                        if( guess->qfamily->isIdenticalTo(*tribe.qfamily) )
                        {
                            return guess;
                        }
                    }

                    return 0;
                }

                bool FoundSamePostureThan(const Posture &p, const Tribe::List &l) noexcept
                {
                    for(const Tribe *t=l.head;t;t=t->next)
                    {
                        const Posture &_ = *t;
                        if( _ == p ) return true;
                    }
                    return false;
                }

                void Tribes:: removeFutile(XMLog &xml)
                {
                    Tribe::List active;
                    while(size>0)
                    {
                        Tribe * const tribe = popHead();
                        if( tribe->isFutile() )
                        {
                            delete tribe;
                            continue;
                        }
                        active.pushTail(tribe);
                    }
                    swapWith(active);
                    assert(isSortedBy(Tribe::Compare));
                    Y_XML_COMMENT(xml, "#active    = " << size);
                }

                void Tribes:: findMultiple(XMLog &xml)
                {
                    Tribe::List kept;
                    size_t      multiple = 0;
                    while(size>0)
                    {
                        AutoPtr<Tribe> lhs = popHead();
                        if( FoundSamePostureThan(*lhs,kept) ) {
                            ++multiple;
                            continue;
                        }
                        kept.pushTail( lhs.yield() );
                    }
                    swapWith(kept);
                    assert(isSortedBy(Tribe::Compare));
                    Y_XML_COMMENT(xml, "#multiple  = " << multiple);

                }

                void Tribes:: findMatching(XMLog &xml)
                {
                    size_t      matching = 0;
                CYCLE:
                    {
                        Tribe::List retained;
                        bool        modified = false;
                        while(size>0)
                        {
                            AutoPtr<Tribe> lhs = popHead();
                            Tribe * const  rhs = FindIndenticalFamily(*lhs,retained);
                            if(0!=rhs)
                            {
                                matching++;
                                modified = true;
                                //std::cerr << "---- Identical " << std::endl;
                                //std::cerr << "(*) lhs=" << *lhs << std::endl;
                                //std::cerr << "(*) rhs=" << *rhs << std::endl;

                                const bool samePosture = collapse(*lhs,*rhs);
                                //std::cerr << "--> lhs=" << *lhs << std::endl;
                                //std::cerr << "--> rhs=" << *rhs << std::endl;
                                if( samePosture )
                                {
                                    if(rhs->isFutile())
                                        delete retained.pop(rhs); // drop futile rhs
                                    continue;                     // drop same lhs in any case
                                }

                                // otherwise keep lhs
                            }

                            retained.pushTail( lhs.yield() );
                        }
                        swapWith(retained);
                        assert(isSortedBy(Tribe::Compare));
                        if(modified) goto CYCLE;
                    }
                    Y_XML_COMMENT(xml, "#matching  = " << matching);
                }


                static inline
                Tribe *findReplacementFamilyFor(const Tribe * const tribe)
                {
                    assert(0!=tribe);
                    for(Tribe *guess=tribe->prev;guess;guess=guess->prev)
                    {
                        if(tribe->content==guess->content)
                            return guess;
                    }
                    return 0;
                }

                void Tribes:: replaceBasis(XMLog &xml)
                {
                    size_t replaced = 0;
                    for(Tribe *tribe=head;tribe;tribe=tribe->next)
                    {
                        Tribe * const guess = findReplacementFamilyFor(tribe);
                        if(guess)
                        {
                            tribe->adoptedBy(*guess);
                            ++replaced;
                        }
                    }
                    if( replaced ) MergeSort::Call(*this,Tribe::Compare);
                    Y_XML_COMMENT(xml, "#replaced  = " << replaced);
                }

                void Tribes:: process(XMLog &xml, const unsigned flag, uint64_t * const pEll)
                {
                    Y_XML_COMMENT(xml, "#generated = " << size);
                    Y_XML_COMMENT(xml, "#collected = " << collected);

                    const StopWatch sw(pEll);
                    assert(isSortedBy(Tribe::Compare));

                    if( 0 != (flag&Strategy::RemoveFutile) ) removeFutile(xml);
                    if( 0 != (flag&Strategy::FindMultiple) ) findMultiple(xml);
                    if( 0 != (flag&Strategy::FindMatching) ) findMatching(xml);
                    if( 0 != (flag&Strategy::ReplaceBasis) ) replaceBasis(xml);

                    assert(isSortedBy(Tribe::Compare));

                    // final call for unique signature ?
                    if(size<=0) {
                        MergeSort::Call( Coerce(db), CompareVectors);
                        return;
                    }



                }


            }

        }

    }

}


