
#include "y/apex/api/ortho/coven/tribes.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {
            namespace Coven
            {
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


                static inline
                void RemoveFrom(Tribe::List &tribes, const size_t zid)
                {
                    for(Tribe *tribe=tribes.head;tribe;tribe=tribe->next)
                    {
                        if(!tribe->removed(zid)) throw Specific::Exception(Tribe::CallSign,"missing index=%u", unsigned(zid));
                    }
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


                static inline
                const Tribe * FoundDuplicateOf(const Vector      &vec,
                                                const Tribe::List &ok) noexcept
                {
                    for(const Tribe *guess=ok.head;guess;guess=guess->next)
                    {
                        assert(0!=guess->lastVec);
                        if( vec == *(guess->lastVec) ) return guess;
                    }
                    return 0;
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

                    // initialize for collected
                    assert(db.size==size);
                    Coerce(collected) = db.size;
                }


            }


        }

    }

}


