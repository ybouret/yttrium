
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
                //______________________________________________________________
                //
                //
                //
                //! List of optimization strategies
                //
                //
                //______________________________________________________________
                struct Strategy
                {
                    static const unsigned RemoveFutile = 0x01; //!< remove futile tribes (won't produce more vectors)
                    static const unsigned FindMultiple = 0x02; //!< find and deal with multiple same content
                    static const unsigned FindMatching = 0x04; //!< find and deal with different contents/same families
                    static const unsigned ReplaceBasis = 0x08; //!< replace equivalent basis, for memory
                    static const unsigned HyperClosure = 0x10; //!< anticipate hyperplane
                    static const unsigned Optimize     = 0xff; //!< full optimization
                };

                //______________________________________________________________
                //
                //
                //
                //! Tribe is a Posture used to build a Family
                //
                //
                //______________________________________________________________
                class Tribe : public Quantized, public Posture
                {
                public:
                    //__________________________________________________________
                    //
                    //
                    // Definitions
                    //
                    //__________________________________________________________
                    typedef CxxListOf<Tribe>  List;                       //!< alias
                    static const char * const CallSign;                   //!< "Coven::Tribe"


                    //__________________________________________________________
                    //
                    //
                    // C++
                    //
                    //__________________________________________________________

                    //! intialize
                    /**
                     \param data matrix of row vectors
                     \param bank to handle indices
                     \param indx use data[indx] as primary vector
                     \param qfcc Family/Vector cache
                     */
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

                    //! iterate next tribe
                    /**
                     \param root parent tribe
                     \param data matrix of row vectors
                     \param node node from root's residue to promote content
                     */
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


                    virtual ~Tribe() noexcept; //!< cleanup
                    Y_OSTREAM_PROTO(Tribe);    //!< display

                    //__________________________________________________________
                    //
                    //
                    // Methods
                    //
                    //__________________________________________________________
                    static SignType Compare(const Tribe * const,const Tribe * const) noexcept; //!< compare family weights
                    void            adoptedBy(Tribe &better) noexcept;                         //!< replace family
                    bool            isFutile()         const noexcept;                        //!< Foundation or no residue

                    //! add to sorted children
                    template <typename MATRIX> inline
                    void progeny(List          & chld,
                                 const MATRIX  & data,
                                 const unsigned  flag)
                    {
                        assert( chld.isSortedBy(Compare) );

                        //------------------------------------------------------
                        //
                        // without hypeclosure, default lineage
                        //
                        //------------------------------------------------------
                        if(0== (flag&Strategy::HyperClosure) ) { lineage(chld,data); return; }

                        //------------------------------------------------------
                        //
                        // Hyperclosure
                        //
                        //------------------------------------------------------
                        switch(qfamily->quality)
                        {
                            case Degenerate: throwDegenerate();  return;
                            case Foundation: flush();            return;
                            case Fragmental: lineage(chld,data); return ;
                            case Hyperplane:
                                while(residue.size>0)
                                {
                                    Tribe *tribe = new Tribe(*this,data,residue.head);
                                    residue.cutHead();
                                    if(0!=tribe->lastVec) {
                                        assert(Foundation==tribe->qfamily->quality);
                                        tribe->flush();
                                        ListOps::InsertOrdered(chld,tribe,Compare);
                                        assert( chld.isSortedBy(Compare) );
                                        return;
                                    }
                                    else
                                        delete tribe;
                                }
                                assert(0==residue.size);
                                return;
                        }




                    }

                    //__________________________________________________________
                    //
                    //
                    // Members
                    //
                    //__________________________________________________________
                    FCache               qfcache; //!< family/vector cache
                    Family *             qfamily; //!< current family
                    const size_t         lastIdx; //!< last used index
                    const Vector * const lastVec; //!< last created vector
                    Tribe *              next;    //!< for list
                    Tribe *              prev;    //!< for list

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Tribe);
                    void destroy()      noexcept; //!< destroy family
                    void throwDegenerate() const; //!< raise error

                    //! default lineage creation
                    template <typename MATRIX> inline
                    void lineage(List          & chld,
                                 const MATRIX  & data)
                    {

                        {
                            List here;
                            for(const INode *node=residue.head;node;node=node->next)
                            {
                                here.pushTail( new Tribe(*this,data,node) );
                            }
                            MergeSort::Call(here,Compare);
                            ListOps::Fusion(chld,here,Compare);
                        }
                        assert( chld.isSortedBy(Compare) );
                    }

                    //! setup initial family from a vector
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

