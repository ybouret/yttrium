
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
                //______________________________________________________________
                //
                //
                //
                //! Callback to apply on newly created, unique vector
                //
                //
                //______________________________________________________________
                typedef Functor<void,TL1(const Vector&)> Callback;

                //______________________________________________________________
                //
                //
                //
                //! list of Tribes
                //
                //
                //______________________________________________________________
                class Tribes : public Tribe::List
                {
                public:
                    //__________________________________________________________
                    //
                    //
                    // Definitions
                    //
                    //__________________________________________________________
                    static const char * const CallSign;                 //!< "Coven::Tribes"
                    static Natural            MaxCount(const size_t n); //!< sum of arrangements
                    static void               Display(const Vector &v); //!< helper for default callback

                    static const unsigned RemoveFutile = 0x01;

                    //__________________________________________________________
                    //
                    //
                    // C++
                    //
                    //__________________________________________________________

                    //! setup from initial set of vectors
                    /**
                     \param xml  for output
                     \param proc callback for vectors
                     \param data matrix of row vectors
                     \param bank for indices handling
                     \param qfcc for vectors/families
                     */
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

                        // create all possible initial tribes
                        {
                            const size_t n = data.rows;
                            for(size_t indx=1;indx<=n;++indx)
                                (void) pushTail( new Tribe(data,bank,indx,qfcc) );
                        }

                        // remove null and duplicates, sort tribes and collect vectors
                        doInitialize(xml,proc);
                    }

                    virtual ~Tribes() noexcept; //!< cleanup
                    Y_OSTREAM_PROTO(Tribes);    //!< display

                    //__________________________________________________________
                    //
                    //
                    // Methods
                    //
                    //__________________________________________________________

                    //! get signature from created vectors
                    Digest          signature(Hashing::Function &) const;

                    //! compare vectors for unique db
                    static SignType CompareVectors(const Vector * const lhs,
                                                   const Vector * const rhs) noexcept;

                    //! create new generation
                    /**
                     - create all possible children
                     - collect all newly created vectors
                     - remove tribes that won't produce new vectors
                     - use equivalent families to reduce memory and complexity
                     \param xml for output
                     \param proc to apply on newly created vectors
                     \param data matrix of row vectors
                     \param flag modification to raw algorithm
                     */
                    template <typename MATRIX> inline
                    void generate(XMLog        & xml,
                                  Callback     & proc,
                                  const MATRIX & data,
                                  const unsigned flag)
                    {
                        ++Coerce(iteration);
                        Y_XML_SECTION_OPT(xml, "Coven::Tribes", "iteration=" << iteration << " size=" << size);

                        // create all possible children
                        assert(isSortedAccordingTo(Tribe::Compare));
                        {
                            Tribe::List chld;
                            for(Tribe *tribe=head;tribe;tribe=tribe->next)
                                tribe->progeny(chld,data);
                            swapWith(chld);
                        }
                        assert(isSortedAccordingTo(Tribe::Compare));

                        // collect all newly created vectors
                        collect(proc);

                        // process
                        process(xml,flag);
                    }



                    //__________________________________________________________
                    //
                    //
                    // Members
                    //
                    //__________________________________________________________
                    const size_t iteration; //!< current iteration
                    const size_t collected; //!< collected at this iteration

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Tribes);
                    VCache             vc;
                public:
                    const Vector::List db; //!< current database, sorted when done

                private:
                    void           doInitialize(XMLog &, Callback &);
                    void           noNullVector(XMLog &);
                    void           noDuplicates(XMLog &);
                    const Vector * tryInsertNew(const Vector &vec);
                    
                    void           collect(Callback &);
                    void           process(XMLog &,const unsigned);


                };



            }

        }

    }

}

#endif
