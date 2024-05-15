
//! \file

#ifndef Y_Chemical_Equalizer_Included
#define Y_Chemical_Equalizer_Included 1

#include "y/chemical/reactive/clusters.hpp"
#include "y/container/cxx/array.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Equalizer
        {
        public:
            explicit Equalizer(const Clusters &);
            virtual ~Equalizer() noexcept;
            
            void tune(XWritable     &C0,
                      const Cluster &cluster,
                      XMLog         &xml);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equalizer);
            XMatrixType                 Ceqz;
            Banks                       banks;
            SRepo                       negative;
            CxxArray<Fence,MemoryModel> fences;
        };
    }

}

#endif
