
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



                void Tribes:: process(XMLog &xml, const unsigned flag)
                {
                    Y_XML_COMMENT(xml, "#generated = " << size);
                    Y_XML_COMMENT(xml, "#collected = " << collected);

                    //----------------------------------------------------------
                    //
                    // first pass: removeFutile
                    //
                    //----------------------------------------------------------
                    if( 0 != (flag&RemoveFutile) )
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
                        assert(isSortedAccordingTo(Tribe::Compare));
                        Y_XML_COMMENT(xml, "#active    = " << size);
                    }

                    //----------------------------------------------------------
                    //
                    // second pass: findMultiple
                    //
                    //----------------------------------------------------------
                    if( 0 != (flag&FindMultiple) )
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
                        assert(isSortedAccordingTo(Tribe::Compare));
                        Y_XML_COMMENT(xml, "#multiple  = " << multiple);

                    }


                    //----------------------------------------------------------
                    //
                    // second pass: findMatching
                    //
                    //----------------------------------------------------------
                    if( 0 != (flag&FindMatching) )
                    {
                        Tribe::List kept;
                        size_t      drop = 0;
                        while(size>0)
                        {
                            AutoPtr<Tribe> lhs = popHead();
                            Tribe * const  rhs = FindIndenticalFamily(*lhs,kept);
                            if(0!=rhs)
                            {
                                std::cerr << "---- Identical " << std::endl;
                                std::cerr << "(*) lhs=" << *lhs << std::endl;
                                std::cerr << "(*) rhs=" << *rhs << std::endl;

                                collapse(*lhs,*rhs);
                                std::cerr << "--> lhs=" << *lhs << std::endl;
                                std::cerr << "--> rhs=" << *rhs << std::endl;


                                throw Exception("TODO");
                            }

                            kept.pushTail( lhs.yield() );
                        }
                        swapWith(kept);
                        assert(isSortedAccordingTo(Tribe::Compare));
                        Y_XML_COMMENT(xml, "#matching  = " << drop);
                    }


                    if(size<=0)
                        MergeSort::Call( Coerce(db), CompareVectors);

                }


            }

        }

    }

}


