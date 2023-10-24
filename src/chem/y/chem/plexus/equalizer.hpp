
//! \file

#ifndef Y_Chemical_Equalizer_Included
#define Y_Chemical_Equalizer_Included 1

#include "y/chem/plexus/custodian.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Equalize legalized concentrations
        //
        //
        //______________________________________________________________________
        class Equalizer
        {
        public:
            explicit Equalizer();
            virtual ~Equalizer() noexcept;

            void run(const Cluster   &cluster,
                     Writable<xreal> &Corg,
                     XMLog           &xml);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equalizer);
            class Code;
            Code *code;
        };

    }

}

#endif

