
#ifndef Y_MKL_ParTao1_Included
#define Y_MKL_ParTao1_Included 1


#include "y/mkl/api.hpp"
#include "y/mkl/antelope/add.hpp"
#include "y/mkl/tao/carver.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace Tao
        {

            namespace Parallel
            {

                template <typename TARGET, typename SOURCE> inline
                 void Load(Engine1D &range, TARGET &target, SOURCE &source)
                {
                    for(size_t i=range.latest;i>=range.offset;--i)
                        target[i] = source[i];
                }

            }

            //__________________________________________________________________
            //
            //! target[1..target.size()] = source[1..target.size()]
            //__________________________________________________________________
            template <typename TARGET, typename SOURCE>   inline
            void Load(TARGET &target, SOURCE &source, Carver &carver)
            {
                assert(target.size()<=source.size());
                carver.setup(target.size());
                carver.in1d(Parallel::Load<TARGET,SOURCE>,target,source);
            }

        }

        namespace Tao
        {
            namespace Parallel
            {
                template <typename TARGET, typename SOURCE> inline
                void Save(Engine1D &range, TARGET &target, SOURCE &source)
                {
                    for(size_t i=range.latest;i>=range.offset;--i)
                        target[i] = source[i];
                }

            }

            //__________________________________________________________________
            //
            //! target[1..source.size()] = source[1..source.size()]
            //__________________________________________________________________
            template <typename TARGET, typename SOURCE>   inline
            void Save(TARGET &target, SOURCE &source, Carver &carver)
            {
                assert(target.size()>=source.size());
                carver.setup(source.size());
                carver.in1d(Parallel::Save<TARGET,SOURCE>,target,source);
            }

        }


        namespace Tao
        {
            namespace Parallel
            {
                template <typename TARGET, typename SOURCE> inline
                void Add(Engine1D &range, TARGET &target, SOURCE &source)
                {
                    for(size_t i=range.latest;i>=range.offset;--i)
                        target[i] += source[i];
                }

            }

            //__________________________________________________________________
            //
            //! target += source
            //__________________________________________________________________
            template <typename TARGET, typename SOURCE>   inline
            void Add( TARGET &target, SOURCE &source, Carver &carver)
            {
                assert(target.size()==source.size());
                carver.setup(source.size());
                carver.in1d(Parallel::Add<TARGET,SOURCE>,target,source);
            }

        }



    }

}

#endif

