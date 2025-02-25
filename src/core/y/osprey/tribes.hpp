
//! \file

#ifndef Y_Osprey_Tribes_Included
#define Y_Osprey_Tribes_Included 1

#include "y/osprey/tribe.hpp"
#include "y/functor.hpp"
#include "y/memory/digest.hpp"
#include "y/sort/merge.hpp"
#include "y/exception.hpp"

namespace Yttrium
{
    namespace Osprey
    {
        //! callback upon new vector
        typedef Functor<void,TL1(const QVector &)> Callback;

        //______________________________________________________________________
        //
        //
        //
        //! List of Tribes per generation
        //
        //
        //______________________________________________________________________
        class Tribes : public Proxy<const Tribe::List>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static Apex::Natural MaxCount(const size_t n); //!< max theoretical combinations
            static void          Display(const QVector &); //!< for default Callback


            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //! setup
            /**

             */
            //__________________________________________________________________
            template <typename MATRIX> inline
            explicit Tribes(XMLog         & xml,
                            Callback      & proc,
                            const MATRIX  & data,
                            const IBank   & bank,
                            const QFCache & qfcc) :
            my(),
            fc(qfcc),
            vc(fc->vcache),
            db()
            {
                Y_XML_SECTION_OPT(xml, "Osprey::Tribes","matrix[" << data.rows << "][" << data.cols << "]" );
                //--------------------------------------------------------------
                // initialize all vectors
                //--------------------------------------------------------------
                {
                    const size_t rows = data.rows;
                    for(size_t indx=1;indx<=rows;++indx)
                    {
                        (void) my.pushTail( new Tribe(data,bank,qfcc,indx) )->lastVec;
                    }
                }

                //--------------------------------------------------------------
                // take care of null vector(s)
                //--------------------------------------------------------------
                noNullVec(xml);

                //--------------------------------------------------------------
                // process and detect trivially similar vectors
                //--------------------------------------------------------------
                noReplica(xml,proc);

                Y_XML_COMMENT(xml, "#vec = " << my.size << " in " << vc->dimensions << "d");
            }

            virtual ~Tribes() noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Tribes);    //!< display



            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            static inline SignType QVectorCompare(const QVector * const lhs,
                                                  const QVector * const rhs) noexcept
            {
                return QVector:: Compare(*lhs,*rhs);
            }



            //! create new generation
            template <typename MATRIX> inline
            void generate(XMLog &        xml,
                          Callback &     proc,
                          const MATRIX & data,
                          const unsigned flag)
            {
                Y_XML_SECTION_OPT(xml, "Osprey::Generate", "tribes=" << my.size);

                //--------------------------------------------------------------
                //
                // create new generation
                //
                //--------------------------------------------------------------
                {
                    const bool  optimizeHyperplanes = 0 != (flag & Tribe::OptimizeHyperplanes);
                    Tribe::List newGen;
                    for(Tribe *tribe=my.head;tribe;tribe=tribe->next)
                    {
                        tribe->unfold(newGen,data,optimizeHyperplanes);
                    }
                    my.swapWith(newGen);
                }
                Y_XMLOG(xml, "#generated = " << my.size);

                //--------------------------------------------------------------
                //
                // check if done
                //
                //--------------------------------------------------------------
                if(my.size<=0) {
                    MergeSort::Call(Coerce(db),QVectorCompare);
                    return;
                }

                //--------------------------------------------------------------
                //
                // post-process
                //
                //--------------------------------------------------------------
                makeTribesOrder();
                storeNewVectors(proc);

                //--------------------------------------------------------------
                //
                // post-process
                //
                //--------------------------------------------------------------
                if( 0 == (flag&Tribe::UseBasisCompression) ) return;
                size_t replacement = 0;
                size_t compression = 0;
            CYCLE:
                for(Tribe *tribe = my.head; tribe; tribe=tribe->next)
                {
                    for(const Tribe *guess=tribe->prev;guess;guess=guess->prev)
                    {
                        if( (tribe->qfamily != guess->qfamily) && IList::AreEqual( *(tribe->posture.content),*(guess->posture.content) ) )
                        {
                            assert( tribe->qfamily->hasSameSpanThan( *(guess->qfamily) ) );
                            ++replacement;
                            tribe->replaceFamilyBy(*guess);
                            break;
                        }

                        if( (tribe->qfamily != guess->qfamily) && tribe->qfamily->hasSameSpanThan( *(guess->qfamily) ) )
                        {
                            std::cerr << "---- Same Spans ----" << std::endl;
                            std::cerr << "tribe=" << *tribe << std::endl;
                            std::cerr << "guess=" << *guess << std::endl;
                            Posture lhs( tribe->posture );
                            Posture rhs( guess->posture );
                            std::cerr << "upgrading " << lhs << " and " << rhs << std::endl;
                            throw Exception("Same Spans With Different families!");
                            ++compression;
                            goto CYCLE;
                        }
                    }
                }

                Y_XMLOG(xml, "#replacement = " << replacement);
                Y_XMLOG(xml, "#compression = " << compression);


#if 0
                size_t replacement = 0;
                size_t compression = 0;

                for(Tribe *tribe=my.head;tribe;tribe=tribe->next)
                {
                    for(const Tribe *guess=tribe->prev;guess;guess=guess->prev)
                    {
                        //------------------------------------------------------
                        // check same content => same family
                        //------------------------------------------------------
                        assert(guess->qfamily != tribe->qfamily);
                        if( IList::AreEqual( *(tribe->posture.content),*(guess->posture.content) ) )
                        {
                            assert( tribe->qfamily->hasSameSpanThan( *(guess->qfamily) ) );
                            ++replacement;
                            tribe->replaceFamilyBy(*guess);
                            break;
                        }

                        assert(guess->qfamily != tribe->qfamily);
                        if( tribe->qfamily->hasSameSpanThan( *(guess->qfamily) ) )
                        {
                            std::cerr << "---- Same Spans ----" << std::endl;
                            std::cerr << "tribe=" << *tribe << std::endl;
                            std::cerr << "guess=" << *guess << std::endl;
                            throw Exception("Same Spans With Different families!");
                            ++compression;
                            break;
                        }


                    }
                }

                Y_XMLOG(xml, "#replacement = " << replacement);
                Y_XMLOG(xml, "#compression = " << compression);
#endif

            }


            //! database signature
            Digest signature(Hashing::Function &) const;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tribes);
            Y_PROXY_DECL();
            Tribe::List         my; //!< current list
            QFCache             fc; //!< shared family cache
            QVCache             vc; //!< shared vector cache
        public:
            const QVector::List db; //!< current database of unique vectors

        private:
            void   noNullVec(XMLog &)    noexcept;                //!< initial no null vector
            void   noReplica(XMLog &, Callback &);                //!< initial no replica

            void   makeTribesOrder();
            void   storeNewVectors(Callback &);


            //! remove zid from residue of tribes
            static void NoNullVec(const size_t zid, Tribe::List &tribes) noexcept;

            //! remove replica indx (must be found in content or residue
            static void NoReplica(const size_t indx, Tribe::List &tribes);



            //! try insert vector in database
            const QVector *tryInsert(const QVector &rhs)
            {
                for(const QVector *lhs=db.head;lhs;lhs=lhs->next)
                {
                    if( *lhs == rhs ) return 0;
                }
                return Coerce(db).pushTail( vc->query(rhs) );
            }
        };


    }

}

#endif

