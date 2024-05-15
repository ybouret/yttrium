
//! \file

#ifndef Y_Chemical_Equalizer_Included
#define Y_Chemical_Equalizer_Included 1

#include "y/chemical/reactive/clusters.hpp"
#include "y/container/cxx/array.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! equalize negative concentrations
        //
        //
        //______________________________________________________________________
        class Equalizer
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! prepare for a list of clusters
            explicit Equalizer(const Clusters &);

            //! cleanup
            virtual ~Equalizer() noexcept;
            
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! tune
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
