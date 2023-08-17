
//! \file

#ifndef Y_CamEO_Add_Dire_Included
#define Y_CamEO_Add_Dire_Included 1


#include "y/ordered/dire.hpp"
#include "y/ordered/core/flexible-raw-buffer.hpp"
#include "y/ordered/core/compiled-raw-buffer.hpp"

namespace Yttrium
{

    namespace MKL
    {
        namespace Cameo
        {

            template <typename T>
            struct DirePolicy
            {
                template <typename ALLOCATOR>
                struct FlexibleUnits
                {
                    typedef Dire<T,Core::FlexibleRawBuffer<T,ALLOCATOR> > Type;
                };

                template <size_t N>
                struct CompiledUnits
                {
                    typedef Dire<T,Core::CompiledRawBuffer<N,T> > Type;
                };

                template <typename UNITS> static inline
                T Reduce(UNITS &units)
                {
                    assert(units.size()>1);
                    T sum = units[1];
                    for(size_t i=units.size();i>1;--i) sum += units[i];
                    units.free();
                    return sum;
                }

            };
        }

    }

}

#endif

