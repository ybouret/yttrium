//! \file

#ifndef Y_Osprey_Tribe_Included
#define Y_Osprey_Tribe_Included 1

#include "y/osprey/posture.hpp"
#include "y/apex/api/ortho/family.hpp"
#include "y/data/list/cxx.hpp"
#include "y/stream/xmlog.hpp"


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
            lastVec(addWith(data[lastIdx])),
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
            lastVec(addWith(data[lastIdx])),
            next(0),
            prev(0)
            {
                while(0==lastVec)
                {
                    std::cerr << "found a dependent vector from " << root.posture <<  ", now " << posture << std::endl;
                    //exit(0);
                    break;
                }


            }

            virtual ~Tribe() noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Tribe);    //!< display


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! unfold next generation
            template <typename MATRIX> inline
            void unfold(XMLog &xml, Tribe::List &tribes, const MATRIX &data)
            {
                assert(0!=qfamily);

                // early return on depleted residue
                if(posture.residue->size<=0) return;
                const Apex::Ortho::Quality q   = qfamily->quality;
                const char * const         qid = QMetrics::QualityText(q);
                
                // process according to current quality
                switch(q)
                {
                    case Apex::Ortho::Degenerate: throwDegenerateFamily(qid); return; // error, should not happen
                    case Apex::Ortho::Generating: posture.flush();            return; // nothing to add
                    case Apex::Ortho::Hyperplane:
                    case Apex::Ortho::Fragmental:
                        break;
                }

                
                // generic processing
                for(const INode *node=posture.residue->head;node;node=node->next)
                {
                    tribes.pushTail( new Tribe(data,*this,node) );
                }
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

            template <typename T>
            const  QVector * addWith(const Readable<T> &a)
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

            void throwDegenerateFamily(const char * const qid) const;
        };
    }

}

#endif

