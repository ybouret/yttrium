
//! \file

#ifndef Y_Apex_Ortho_Coven_Tribes_Included
#define Y_Apex_Ortho_Coven_Tribes_Included 1

#include "y/apex/api/ortho/coven/tribe.hpp"
#include "y/stream/xmlog.hpp"
#include "y/memory/digest.hpp"
#include "y/functor.hpp"
#include "y/system/stopwatch.hpp"

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
                     \param pEll ellapsed ticks, optional
                     */
                    template <typename MATRIX> inline
                    explicit Tribes(XMLog &          xml,
                                    Callback     &   proc,
                                    const MATRIX &   data,
                                    const IBank  &   bank,
                                    const FCache &   qfcc,
                                    uint64_t * const pEll = 0) :
                    Tribe::List(),
                    iteration(1),
                    collected(0),
                    vc(qfcc->vcache),
                    db()
                    {
                        Y_XML_SECTION_OPT(xml, "Coven::Tribes", "initialize [" << data.rows << "][" << data.cols << "]");
                        //------------------------------------------------------
                        //
                        // create all possible initial tribes
                        //
                        //------------------------------------------------------
                        {
                            Y_StopWatch(pEll);
                            const size_t    n = data.rows;
                            for(size_t indx=1;indx<=n;++indx)
                                (void) pushTail( new Tribe(data,bank,indx,qfcc) );
                        }

                        //------------------------------------------------------
                        //
                        // remove null and duplicates,
                        // sort tribes and collect vectors
                        //
                        //------------------------------------------------------
                        doInitialize(xml,proc,pEll);
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
                     \param pEll ellapsed ticks, optional
                     */
                    template <typename MATRIX> inline
                    void generate(XMLog        &   xml,
                                  Callback     &   proc,
                                  const MATRIX &   data,
                                  const unsigned   flag,
                                  uint64_t * const pEll = 0)
                    {
                        ++Coerce(iteration);
                        Y_XML_SECTION_OPT(xml, "Coven::Tribes", "iteration=" << iteration << " size=" << size);

                        //------------------------------------------------------
                        //
                        // create all possible children
                        //
                        //------------------------------------------------------
                        {
                            assert(isSortedAccordingTo(Tribe::Compare));
                            const StopWatch sw(pEll);
                            {
                                Tribe::List chld;
                                for(Tribe *tribe=head;tribe;tribe=tribe->next)
                                    tribe->progeny(chld,data,flag);
                                swapWith(chld);
                            }
                            assert(isSortedAccordingTo(Tribe::Compare));
                        }
                        //------------------------------------------------------
                        //
                        // collect all newly created vectors
                        //
                        //------------------------------------------------------
                        collect(proc,pEll);

                        //------------------------------------------------------
                        //
                        // process with flags
                        //
                        //------------------------------------------------------
                        process(xml,flag,pEll);
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
                    void           doInitialize(XMLog &, Callback &, uint64_t * const pEll);
                    void           noNullVector(XMLog &);
                    void           noDuplicates(XMLog &);
                    const Vector * tryInsertNew(const Vector &vec);
                    
                    void           collect(Callback &, uint64_t * const pEll);
                    void           process(XMLog &, const unsigned, uint64_t * const pEll);
                    void           removeFutile(XMLog &);
                    void           findMultiple(XMLog &);
                    void           findMatching(XMLog &);
                    void           replaceBasis(XMLog &);
                };



            }

        }

    }

}

#endif
