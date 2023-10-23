
//! \file

#ifndef Y_Chemical_Custodian_Included
#define Y_Chemical_Custodian_Included 1

#include "y/chem/plexus.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Custodian
        {
        public:
            explicit Custodian();
            virtual ~Custodian() noexcept;

            //! prepare accumulator for each species conserved species
            void prepare(const Plexus &, XMLog &);

            //! cleanup all accumulators
            void enter() noexcept;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Custodian);
            class Code;
            Code *code;
        };

    }

}

#endif

