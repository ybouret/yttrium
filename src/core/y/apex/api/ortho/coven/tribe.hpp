
//! \file

#ifndef Y_Apex_Ortho_Coven_Tribe_Included
#define Y_Apex_Ortho_Coven_Tribe_Included 1

#include "y/apex/api/ortho/coven/posture.hpp"
#include "y/apex/api/ortho/family.hpp"
#include "y/data/list/cxx.hpp"
#include "y/sort/merge.hpp"

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
                    static const unsigned OptimizeHyperPlanes = 0x01;
                    static const unsigned UseBasisCompression = 0x02;


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


                    virtual ~Tribe() noexcept
                    {
                        destroy();
                    }

                    Y_OSTREAM_PROTO(Tribe);

                    static SignType Compare(const Tribe * const lhs, const Tribe * const rhs) noexcept
                    {
                        return Natural::Compare(lhs->qfamily->weight(),rhs->qfamily->weight());
                    }

                    //! add to ordered children
                    template <typename MATRIX> inline
                    void progeny(List          & chld,
                                 const MATRIX  & data,
                                 const unsigned  flag)
                    {

                        assert( chld.isSortedAccordingTo(Tribe::Compare) );
                        {
                            List here;
                            for(const INode *node=residue.head;node;node=node->next)
                            {
                                here.pushTail( new Tribe(*this,data,node) );
                            }
                            MergeSort::Call(here,Compare);
                            ListOps::Fusion(chld,here,Compare);
                        }
                        assert( chld.isSortedAccordingTo(Tribe::Compare) );
                    }

                    void replaceFamilyByFamilyOf(Tribe &better) noexcept
                    {
                        assert(qfamily!=better.qfamily);
                        destroy();
                        (qfamily=better.qfamily)->withhold();
                    }

                    FCache               qfcache;
                    Family *             qfamily;
                    const size_t         lastIdx;
                    const Vector * const lastVec;
                    Tribe *              next;
                    Tribe *              prev;

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Tribe);



                    template <typename READABLE> inline
                    const Vector * tryIncreaseWith(READABLE &a)
                    {
                        try {
                            qfamily->withhold();
                            return qfamily->tryIncreaseWith(a);
                        }
                        catch(...) { destroy(); throw; }
                    }

                    void destroy() noexcept {
                        assert( 0 != qfamily );
                        if( qfamily->liberate() ) qfcache->store(qfamily);
                        qfamily = 0;
                    }
                };


                std::ostream & operator<<(std::ostream &os, const Tribe &tribe)
                {
                    const Posture &posture = tribe;
                    os << posture << "->" << *(tribe.qfamily);
                    return os;
                }
            }
        }
    }
}

#endif

