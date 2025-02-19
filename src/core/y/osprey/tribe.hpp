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

        typedef Apex::Ortho::Metrics QMetrics;
        typedef Apex::Ortho::Vector  QVector;
        typedef Apex::Ortho::Family  QFamily;
        typedef Apex::Ortho::VCache  QVCache;
        typedef Apex::Ortho::FCache  QFCache;

        class Tribe : public Object
        {
        public:
            typedef CxxListOf<Tribe> List;
            static const char * const CallSign;

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

            // building from node from root's residue
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
                if(0==lastVec)
                {
                    std::cerr << "root=" << root << std::endl;
                    std::cerr << std::endl << " Unfolding Dependent Vector!!" << std::endl;
                    exit(0);
                }
            }

            virtual ~Tribe() noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Tribe);



            template <typename MATRIX> inline
            void unfold(XMLog &xml, Tribe::List &tribes, const MATRIX &data)
            {
                assert(0!=qfamily);

                // early return on depleted residue
                if(posture.residue->size<=0) return;
                const Apex::Ortho::Quality q   = qfamily->quality;
                const char * const         qid = QMetrics::QualityText(q);
                //Y_XML_SECTION_OPT(xml,"Tribe::Unfold",qid << " family");

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


            // members
            Posture        posture;
            QFCache        qfcache;
            QFamily       *qfamily;
            const size_t   lastIdx;
            const QVector *lastVec;
            Tribe         *next;
            Tribe         *prev;

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

