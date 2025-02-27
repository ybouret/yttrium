#include "y/apex/api/ortho/coven/tribes.hpp"

namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {
            namespace Coven
            {
                const char * const Tribes:: CallSign = "Coven::Tribes";

                std::ostream & operator<<(std::ostream &os, const Tribes &self)
                {
                    os << '{' << '#' << self.size;
                    if(self.size>0)
                    {
                        os << std::endl;
                        for(const Tribe *tribe=self.head;tribe;tribe=tribe->next)
                        {
                            os << "  " << *tribe << std::endl;
                        }

                    }
                    os << '}';
                    return os;
                }


                Natural Tribes:: MaxCount(const size_t n)
                {
                    Natural res = 0;
                    for(size_t k=1;k<=n;++k) res += Natural::Arrange(n,k);
                    return res;
                }

                void Tribes:: Display(const Vector &v)
                {
                    std::cerr << "\t(+) " << v << std::endl;
                }

                Tribes:: ~Tribes() noexcept
                {
                    while(db.size) vc->store( Coerce(db).popTail() );
                }


                const Vector * Tribes:: tryInsertNew(const Vector &vec)
                {
                    for(const Vector *v=db.head;v;v=v->next)
                    {
                        switch( Vector::Compare(*v,vec) )
                        {
                            case __Zero__: return 0;
                            case Negative:
                            case Positive:
                                continue;
                        }
                    }
                    return Coerce(db).pushTail( vc->query(vec) );
                }

                SignType Tribes:: CompareVectors(const Vector * const lhs,
                                                 const Vector * const rhs) noexcept
                {
                    return Vector::Compare(*lhs,*rhs);
                }

                Digest Tribes:: signature(Hashing::Function &H) const
                {
                    H.set();
                    {
                        HashingStream fp(H);
                        for(const Vector *v=db.head;v;v=v->next) v->serialize(fp);
                    }
                    return Hashing::MD::Of(H);
                }

                void Tribes:: makeReplacement(XMLog &xml)
                {
                    assert( isSortedAccordingTo(Tribe::Compare) );
                    size_t replacement = 0;
                    for(Tribe *curr=head;curr;curr=curr->next)
                    {
                        assert(Foundation != curr->qfamily->quality );
                        for(Tribe *prev=curr->prev;prev;prev=prev->prev)
                        {
                            if( (curr->qfamily!=prev->qfamily) && (curr->content==prev->content) )
                            {
                                curr->adoptedBy(*prev);
                                ++replacement;
                            }
                        }
                    }

                    Y_XML_COMMENT(xml,"#replacement = " << replacement);
                    if(replacement<=0) { assert( isSortedAccordingTo(Tribe::Compare) ); return; }
                    MergeSort::Call(*this,Tribe::Compare);
                }


                void Tribes:: assembleLast(Callback &proc)
                {
                    size_t & count = Coerce(collected);
                    count = 0;
                    for(const Tribe *tribe=head;tribe;tribe=tribe->next)
                    {
                        const Vector *pVector = tribe->lastVec;         if(0==pVector) continue;
                        const Vector *pSingle = tryInsertNew(*pVector); if(0==pSingle) continue;
                        proc(*pSingle);
                        ++count;
                    }
                }


                void Tribes:: RemoveFrom(Tribe::List &tribes, const size_t zid)
                {
                    for(Tribe *tribe=tribes.head;tribe;tribe=tribe->next)
                    {
                        if(!tribe->removed(zid)) throw Specific::Exception(CallSign,"missing index=%u in Tribe", unsigned(zid));
                    }
                }

                const Tribe * Tribes:: FoundDuplicateOf(const Vector      &vec,
                                                        const Tribe::List &ok) noexcept
                {
                    for(const Tribe *guess=ok.head;guess;guess=guess->next)
                    {
                        assert(0!=guess->lastVec);
                        if( vec == *(guess->lastVec) ) return guess;
                    }
                    return 0;
                }

                void Tribes:: noNullVector(XMLog &xml)
                {
                    Tribe::List ok;
                    while(size>0)
                    {
                        Tribe * const tribe = popHead();
                        if(0==tribe->lastVec)
                        {
                            const size_t zid = tribe->lastIdx;
                            Y_XML_COMMENT(xml, "zero vector #" << zid);
                            delete tribe;
                            RemoveFrom(ok,   zid);
                            RemoveFrom(*this,zid);
                        }
                        else
                        {
                            ok.pushTail(tribe);
                        }
                    }
                    swapWith(ok);
                }


                void Tribes:: noDuplicates(XMLog &xml)
                {
                    Tribe::List ok;
                    while(size>0)
                    {
                        Tribe * const        tribe = popHead();assert(0!=tribe->lastVec);
                        const Tribe *  const guess = FoundDuplicateOf(*(tribe->lastVec),ok);

                        if(0!=guess)
                        {
                            const size_t rid = tribe->lastIdx;
                            Y_XML_COMMENT(xml, "duplicates #" << rid << " and #" << guess->lastIdx);
                            RemoveFrom(*this,rid);
                            for(Tribe *tr=ok.head;tr;tr=tr->next)
                            {
                                if(!tr->residue.removed(rid)) throw Specific:: Exception(CallSign, "missing index=%u in Tribe residue", unsigned(rid));
                            }
                            delete tribe;
                        }
                        else
                            ok.pushTail(tribe);
                    }
                    swapWith(ok);
                }

                void Tribes:: doInitialize(XMLog &xml, Callback &proc)
                {
                    noNullVector(xml);
                    noDuplicates(xml);
                    MergeSort::Call(*this,Tribe::Compare);

                    // collect first inserted vectors
                    for(const Tribe *tribe=head;tribe;tribe=tribe->next)
                    {
                        assert(0!=tribe->lastVec);
                        const Vector *vec = tryInsertNew(*(tribe->lastVec));
                        if(!vec) throw Specific::Exception(CallSign,"Unexpected multiple initial vector");
                        proc(*vec);
                    }

                    assert(db.size==size);
                    Coerce(collected) = db.size;
                }

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


                void Tribes:: makeCompression(XMLog &xml)
                {
                    makeReplacement(xml);

                    Tribe::List kept;
                    std::cerr << "compression [";
                    while(size>0)
                    {
                        AutoPtr<Tribe> lhs = popHead();

                        for(Tribe *rhs=kept.head;rhs;rhs=rhs->next)
                        {
                            if(lhs->qfamily==rhs->qfamily) continue;

                            if(lhs->qfamily->isIdenticalTo(*rhs->qfamily))
                            {
                                //std::cerr << "---- Same Vectors, different families" << std::endl;
                                //std::cerr << "(*) lhs=" << *lhs << std::endl;
                                //std::cerr << "(*) rhs=" << *rhs << std::endl;

                                Posture &L = *lhs;
                                Posture &R = *rhs;

                                collapse(L,R);
                                //std::cerr << "--> lhs=" << *lhs << std::endl;
                                //std::cerr << "--> rhs=" << *rhs << std::endl;

                                if(L==R)
                                {
                                    (std::cerr << '#').flush();
                                    continue;
                                }



                                throw Exception("Not Handled!!");
                            }
                        }

                        (std::cerr << '.').flush();
                        kept.pushTail(lhs.yield());
                    }
                    std::cerr << "]" << std::endl;

                    swapWith(kept);

                }


                void Tribes:: removeFutile(XMLog &xml, const unsigned flag)
                {
                    Y_XML_COMMENT(xml, "#generated   = " << size);
                    Y_XML_COMMENT(xml, "#collected   = " << collected);

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



                    if(size<=0)
                        MergeSort::Call( Coerce(db), CompareVectors);
                    else
                    {
                        if( 0 != (flag & Tribe::UseBasisCompression ) )
                            makeCompression(xml);
                    }
                }

            }
        }

    }
}


