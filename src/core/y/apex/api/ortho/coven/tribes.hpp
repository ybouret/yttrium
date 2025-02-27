
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

                    static Natural MaxCount(const size_t n);
                    static void Display(const Vector &v);

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
                        doInitialize(xml,proc);
                    }

                    virtual ~Tribes() noexcept;
                    Y_OSTREAM_PROTO(Tribes);

                    Digest          signature(Hashing::Function &H) const;
                    static SignType CompareVectors(const Vector * const lhs,
                                                   const Vector * const rhs) noexcept;


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
                        assembleLast(proc);
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




                    const size_t iteration;
                    const size_t collected;

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Tribes);
                    VCache             vc;
                public:
                    const Vector::List db;


                    void           doInitialize(XMLog &xml, Callback &proc);
                    void           noNullVector(XMLog &xml);
                    void           noDuplicates(XMLog &xml);
                    const Vector * tryInsertNew(const Vector &vec);
                    void           assembleLast(Callback &);

                    void makeReplacement(XMLog &xml);
                    void makeCompression(XMLog &xml);

                    static void          RemoveFrom(Tribe::List &tribes, const size_t zid);
                    static const Tribe * FoundDuplicateOf(const Vector      &vec, const Tribe::List &ok) noexcept;





                };


            
            }

        }

    }

}

#endif
