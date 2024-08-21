//! \file

#ifndef Y_Chemical_Janitor_Included
#define Y_Chemical_Janitor_Included 1

#include "y/chemical/plexus/janitor/broken.hpp"
#include "y/chemical/plexus/cluster.hpp"
#include "y/chemical/library.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Janitor to fix broken law per cluster
        //
        //
        //______________________________________________________________________
        class Janitor
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Conservation::Laws::Group Group; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Janitor(const Cluster &cl); //!< setup for persistent cluster
            virtual ~Janitor() noexcept;         //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void prolog() noexcept;                              //!< clean all injectect
            void process(XWritable  &Ctop, XMLog &xml);          //!< process all groups
            void epilog(XWritable &dC, const Level L);           //!< load all injected
            void display(std::ostream &, const Library &) const; //!< display injected

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Cluster    &     mine; //!< persistent cluster
            const Group * const    init; //!< if mine.laws.isValid()
            const size_t           rows; //!< laws max group size
            const size_t           cols; //!< max species in sub-level
            Broken::Series         jail; //!< local array of broken laws
            XMatrix                Cnew; //!< workspace(rows,cols) to store Csub
            CxxArray<XAdd,XMemory> Cinj; //!< workspace, store incremental increases
            XAdd                   xadd; //!< for internal additions
            const xreal_t          zero; //!< alias

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Janitor);
            void  process(const Group &grp,
                          XWritable   &C,
                          const Level  L,
                          XMLog       &xml);
        };
    }

}

#endif

