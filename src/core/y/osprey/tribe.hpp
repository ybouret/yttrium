//! \file

#ifndef Y_Osprey_Tribe_Included
#define Y_Osprey_Tribe_Included 1

#include "y/osprey/posture.hpp"
#include "y/apex/api/ortho/family.hpp"
#include "y/data/list/cxx.hpp"
#include "y/stream/xmlog.hpp"
#include "y/sequence/vector.hpp"

namespace Yttrium
{
    namespace Osprey
    {
        //______________________________________________________________________
        //
        //
        // Aliases
        //
        //______________________________________________________________________
        typedef Apex::Ortho::Metrics QMetrics;  //!< alias
        typedef Apex::Ortho::Vector  QVector;   //!< alias
        typedef Apex::Ortho::Family  QFamily;   //!< alias
        typedef Apex::Ortho::VCache  QVCache;   //!< alias
        typedef Apex::Ortho::FCache  QFCache;   //!< alias

        //______________________________________________________________________
        //
        //
        //
        //! family built from posture=content+residue
        //
        //
        //______________________________________________________________________
        class Tribe : public Object
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef CxxListOf<Tribe>  List;      //!< alias
            static const char * const CallSign;  //!< "Osprey::Tribe"
            static const unsigned     OptimizeHyperplanes = 0x01;
            static const unsigned     UseBasisCompression = 0x02;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! building from data[indx]
            template <typename MATRIX> inline
            explicit Tribe(const MATRIX  & data,
                           const IBank   & bank,
                           const QFCache & qfcc,
                           const size_t    indx) :
            posture(bank,data.rows,indx),
            qfcache(qfcc),
            qfamily(qfcache->query()),
            lastIdx(indx),
            lastVec(constructorIncreaseWith(data[lastIdx])),
            next(0),
            prev(0)
            {

            }

            //! building from node from root's residue
            template <typename MATRIX> inline
            explicit Tribe(const MATRIX  &     data,
                           const Tribe   &     root,
                           const INode * const node) :
            posture(root.posture,node),
            qfcache(root.qfcache),
            qfamily(qfcache->query(*root.qfamily)),
            lastIdx(**node),
            lastVec(constructorIncreaseWith(data[lastIdx])),
            next(0),
            prev(0)
            {

            }

            virtual ~Tribe() noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Tribe);    //!< display


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //
            //! unfold next generation
            //
            //__________________________________________________________________
            template <typename MATRIX> inline
            void unfold(Tribe::List   &tribes,
                        const MATRIX  &data,
                        const bool     optimizeHyperplanes)
            {
                assert(0!=qfamily);

                //--------------------------------------------------------------
                //
                // early return on depleted residue
                //
                //--------------------------------------------------------------
                if(posture.residue->size<=0) return;

                //--------------------------------------------------------------
                //
                // process according to current quality
                //
                //--------------------------------------------------------------
                const Apex::Ortho::Quality q = qfamily->quality;
                switch(q)
                {
                    case Apex::Ortho::Degenerate: throwDegenerateFamily(q); return; // error, should not happen
                    case Apex::Ortho::Generating: posture.flush();          return; // nothing to add, should be specific to 1D
                    case Apex::Ortho::Hyperplane:
                        if(optimizeHyperplanes)
                        {
                            for(const INode *node=posture.residue->head;node;node=node->next)
                            {
                                Tribe *tribe = new Tribe(data,*this,node);
                                if(tribe->lastVec)
                                {
                                    tribes.pushTail( tribe )->posture.flush();
                                    assert(Apex::Ortho::Generating==tribe->qfamily->quality);
                                    break;
                                }
                                else
                                    delete tribe;
                            }
                            return;
                        }
                        else
                            break;

                    case Apex::Ortho::Fragmental:
                        break;
                }

                // default growth
                for(const INode *node=posture.residue->head;node;node=node->next)
                    (void) tribes.pushTail( new Tribe(data,*this,node) );


            }

            //__________________________________________________________________
            //
            //
            // members
            //
            //__________________________________________________________________
            Posture        posture; //!< current posture
            QFCache        qfcache; //!< shared cache
            QFamily       *qfamily; //!< current family
            const size_t   lastIdx; //!< last index in conent
            const QVector *lastVec; //!< last vector from matrix[lastIdx]
            Tribe         *next;    //!< for list
            Tribe         *prev;    //!< for list

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tribe);

            //! increase qfamily in constructor
            /**
             withold qfamily upon completion, destroy and re-throw upon exception
             */
            template <typename T>
            const  QVector * constructorIncreaseWith(const Readable<T> &a)
            {
                try
                {
                    const QVector *ans = qfamily->tryIncreaseWith(a);
                    qfamily->withhold();
                    return ans;
                }
                catch(...) { destroy(); throw; }
            }

            //! destroy qfamily
            void destroy() noexcept;

            void throwDegenerateFamily(const Apex::Ortho::Quality) const;
        };
    }

}

#endif

