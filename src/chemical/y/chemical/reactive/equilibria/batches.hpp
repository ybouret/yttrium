//! \file

#ifndef Y_Chemical_Equilibria_Batches_Included
#define Y_Chemical_Equilibria_Batches_Included 1

#include "y/chemical/reactive/equilibria/batch.hpp"
#include "y/chemical/reactive/equilibria.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! list of equilibria batch
        //
        //
        //______________________________________________________________________
        class Batches : public Batch::List
        {
        public:
            explicit Batches(const Equilibria &eqs); //!< make batches
            virtual ~Batches() noexcept;             //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Batches);
            void gatherSharing(const Equilibria &eqs); //!< main algorithm
            void lookForFusion() noexcept;             //!< algorithm helper
            
        };

    }

}

#endif

