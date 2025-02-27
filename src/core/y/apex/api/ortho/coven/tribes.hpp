
//! \file

#ifndef Y_Apex_Ortho_Coven_Tribes_Included
#define Y_Apex_Ortho_Coven_Tribes_Included 1

#include "y/apex/api/ortho/coven/tribe.hpp"
#include "y/stream/xmlog.hpp"
#include "y/stream/hash/srzh.hpp"
#include "y/stream/hash/output.hpp"
#include "y/memory/digest.hpp"
#include "y/hashing/md.hpp"
#include "y/functor.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {
            namespace Coven
            {
                typedef Functor<void,TL1(const Vector&)> Callback;

                class Tribes : public Tribe::List
                {
                public:
                    static const char * const CallSign;

                    static Natural MaxCount(const size_t n)
                    {
                        Natural res = 0;
                        for(size_t k=1;k<=n;++k) res += Natural::Arrange(n,k);
                        return res;
                    }

                    static void Display(const Vector &v)
                    {
                        std::cerr << "\t(+) " << v << std::endl;
                    }

                    template <typename MATRIX> inline
                    explicit Tribes(XMLog &        xml,
                                    Callback     & proc,
                                    const MATRIX & data,
                                    const IBank  & bank,
                                    const FCache & qfcc) :
                    Tribe::List(),
                    iteration(1),
                    collected(0),
                    vc(qfcc->vcache),
                    db()
                    {
                        Y_XML_SECTION_OPT(xml, "Coven::Tribes", "initialize [" << data.rows << "][" << data.cols << "]");
                        {
                            const size_t n = data.rows;
                            for(size_t indx=1;indx<=n;++indx)
                                (void) pushTail( new Tribe(data,bank,indx,qfcc) );
                        }
                        initialize(xml,proc);
                        assert(db.size==size);
                        Coerce(collected) = db.size;
                    }

                    virtual ~Tribes() noexcept
                    {
                        while(db.size) vc->store( Coerce(db).popTail() );
                    }

                    Y_OSTREAM_PROTO(Tribes);


                    static inline SignType CompareVectors(const Vector * const lhs,
                                                          const Vector * const rhs) noexcept
                    {
                        return Vector::Compare(*lhs,*rhs);
                    }

                    template <typename MATRIX> inline
                    void generate(XMLog        & xml,
                                  Callback     & proc,
                                  const MATRIX & data,
                                  const unsigned flag)
                    {
                        ++Coerce(iteration);
                        Y_XML_SECTION_OPT(xml, "Coven::Tribes", "iteration=" << iteration << " size=" << size);
                        assert(isSortedAccordingTo(Tribe::Compare));
                        {
                            Tribe::List chld;
                            for(Tribe *tribe=head;tribe;tribe=tribe->next)
                                tribe->progeny(chld,data,flag);
                            swapWith(chld);
                        }
                        assert(isSortedAccordingTo(Tribe::Compare));


                        Y_XML_COMMENT(xml, "#generated   = " << size);
                        collect(proc);
                        Y_XML_COMMENT(xml, "#collected   = " << collected);

                        if(size<=0)
                        {
                            MergeSort::Call( Coerce(db), CompareVectors);
                        }
                        else
                        {
                            if( 0 != (flag & Tribe::UseBasisCompression ) )
                                makeCompression(xml);
                        }

                    }

                    Digest signature(Hashing::Function &H) const
                    {
                        H.set();
                        {
                            HashingStream fp(H);
                            for(const Vector *v=db.head;v;v=v->next) v->serialize(fp);
                        }
                        return Hashing::MD::Of(H);
                    }

                    const size_t iteration;;
                    const size_t collected;

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Tribes);
                    VCache             vc;
                public:
                    const Vector::List db;

                    void makeReplacement(XMLog &xml)
                    {
                        assert( isSortedAccordingTo(Tribe::Compare) );
                        size_t replacement = 0;
                        for(Tribe *curr=head;curr;curr=curr->next)
                        {
                            for(Tribe *prev=curr->prev;prev;prev=prev->prev)
                            {
                                if( (curr->qfamily!=prev->qfamily) && (curr->content==prev->content) )
                                {
                                    curr->replaceFamilyByFamilyOf(*prev);
                                    ++replacement;
                                }
                            }
                        }


                        Y_XML_COMMENT(xml,"#replacement = " << replacement);
                        if(replacement<=0) { assert( isSortedAccordingTo(Tribe::Compare) ); return; }
                        MergeSort::Call(*this,Tribe::Compare);
                    }

                    void makeCompression(XMLog &xml)
                    {
                        makeReplacement(xml);
                    }

                    void collect(Callback &proc)
                    {
                        size_t & count = Coerce(collected);
                        count = 0;
                        for(const Tribe *tribe=head;tribe;tribe=tribe->next)
                        {
                            const Vector *pVector = tribe->lastVec;      if(0==pVector) continue;
                            const Vector *pSingle = tryInsert(*pVector); if(0==pSingle) continue;
                            proc(*pSingle);
                            ++count;
                        }
                    }

                    const Vector * tryInsert(const Vector &vec)
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

                    static void RemoveFrom(Tribe::List &tribes, const size_t zid)
                    {
                        for(Tribe *tribe=tribes.head;tribe;tribe=tribe->next)
                        {
                            if(!tribe->removed(zid)) throw Specific::Exception(CallSign,"missing index=%u in Tribe", unsigned(zid));
                        }
                    }

                    void noNullVector(XMLog &xml)
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

                    static const Tribe * FoundDuplicateOf(const Vector      &vec,
                                                          const Tribe::List &ok) noexcept
                    {
                        for(const Tribe *guess=ok.head;guess;guess=guess->next)
                        {
                            assert(0!=guess->lastVec);
                            if( vec == *(guess->lastVec) ) return guess;
                        }
                        return 0;
                    }

                    void noDuplicates(XMLog &xml)
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

                    void initialize(XMLog &xml, Callback &proc)
                    {
                        noNullVector(xml);
                        noDuplicates(xml);
                        MergeSort::Call(*this,Tribe::Compare);
                        assert(isSortedAccordingTo(Tribe::Compare));

                        // collect first
                        for(const Tribe *tribe=head;tribe;tribe=tribe->next)
                        {
                            assert(0!=tribe->lastVec);
                            const Vector *vec = tryInsert(*(tribe->lastVec));
                            if(!vec) throw Exception("unexpected multiple initial vector");
                            proc(*vec);
                        }
                    }


                };


            
            }

        }

    }

}

#endif
