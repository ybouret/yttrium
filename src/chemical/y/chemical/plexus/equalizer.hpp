
//! \file

#ifndef Y_Chemical_Equalizer_Included
#define Y_Chemical_Equalizer_Included 1

#include "y/chemical/reactive/clusters.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Equalizer
        {
        public:
            explicit Equalizer(const Clusters &);
            virtual ~Equalizer() noexcept;
            

            XMatrixType Ceqz;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equalizer);
        };
    }

}

#endif
