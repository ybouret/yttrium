
//! \file

#ifndef Y_Apex_Ortho_Coven_Tribe_Included
#define Y_Apex_Ortho_Coven_Tribe_Included 1

#include "y/apex/api/ortho/coven/posture.hpp"
#include "y/apex/api/ortho/family.hpp"
#include "y/data/list/cxx.hpp"
#include "y/sort/merge.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {
            namespace Coven
            {
                class Tribe : public Quantized, public Posture
                {
                public:

                    typedef CxxListOf<Tribe> List;
                    static const char * const CallSign;
                    static const unsigned     OptimizeHyperPlanes = 0x01;
                    static const unsigned     UseBasisCompression = 0x02;


                    template <typename MATRIX> inline
                    explicit Tribe(const MATRIX & data,
                                   const IBank  & bank,
                                   const size_t   indx,
                                   const FCache & qfcc) :
                    Posture(bank,data.rows,indx),
                    qfcache(qfcc),
                    qfamily( qfcache->query() ),
                    lastIdx( indx ),
                    lastVec( tryIncreaseWith(data[lastIdx] ) ),
                    next(0),
                    prev(0)
                    {

                    }

                    template <typename MATRIX> inline
                    explicit Tribe(const Tribe  &        root,
                                   const MATRIX &        data,
                                   const INode   * const node) :
                    Posture(root,node),
                    qfcache(root.qfcache),
                    qfamily(qfcache->query( *root.qfamily ) ),
                    lastIdx(**node),
                    lastVec( tryIncreaseWith(data[lastIdx] ) ),
                    next(0),
                    prev(0)
                    {
                    }


                    virtual ~Tribe() noexcept;
                    Y_OSTREAM_PROTO(Tribe);

                    static SignType Compare(const Tribe * const lhs, const Tribe * const rhs) noexcept;
                    void replaceFamilyByFamilyOf(Tribe &better) noexcept;

                    //! add to ordered children
                    template <typename MATRIX> inline
                    void progeny(List          & chld,
                                 const MATRIX  & data,
                                 const unsigned  flag)
                    {
                        if(0!= (flag&OptimizeHyperPlanes) )
                        {
                            switch(qfamily->quality)
                            {
                                case Degenerate: throwDegenerate(); return;
                                case Foundation: flush();           return;
                                case Hyperplane:
                                    for(const INode *node=residue.head;node;node=node->next)
                                    {
                                        AutoPtr<Tribe> attempt = new Tribe(*this,data,node);
                                        if(0==attempt->lastVec) continue;
                                        assert(Foundation==attempt->qfamily->quality);
                                        ListOps::InsertOrdered(chld,attempt.yield(),Compare);
                                        assert(chld.isSortedAccordingTo(Compare));
                                        return; // with one new child
                                    }
                                    return; // without new child

                                case Fragmental:
                                    break;
                            }
                            assert( (*qfamily)->size>0 );
                            assert( (*qfamily)->size<qfamily->concluding);
                        }
                        lineage(chld,data);
                    }



                    FCache               qfcache;
                    Family *             qfamily;
                    const size_t         lastIdx;
                    const Vector * const lastVec;
                    Tribe *              next;
                    Tribe *              prev;

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Tribe);
                    void destroy() noexcept;
                    void throwDegenerate() const;

                    template <typename MATRIX> inline
                    void lineage(List          & chld,
                                 const MATRIX  & data)
                    {

                        assert( chld.isSortedAccordingTo(Compare) );
                        {
                            List here;
                            for(const INode *node=residue.head;node;node=node->next)
                            {
                                here.pushTail( new Tribe(*this,data,node) );
                            }
                            MergeSort::Call(here,Compare);
                            ListOps::Fusion(chld,here,Compare);
                        }
                        assert( chld.isSortedAccordingTo(Compare) );
                    }

                    template <typename READABLE> inline
                    const Vector * tryIncreaseWith(READABLE &a)
                    {
                        try {
                            qfamily->withhold();
                            return qfamily->tryIncreaseWith(a);
                        }
                        catch(...) { destroy(); throw; }
                    }

                };


         
            }
        }
    }
}

#endif

