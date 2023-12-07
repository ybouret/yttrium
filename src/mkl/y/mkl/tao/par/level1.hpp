
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
                template <typename TARGET, typename SOURCE>
                struct Load
                {
                    inline void operator()(Motor1D &range,
                                           TARGET  &target,
                                           SOURCE  &source)
                    {
                        {
                            Y_LOCK(range.sync());
                            assert(Concurrent::ForLoopIncrease==range.family);
                            assert(range.offset>0);
                            std::cerr << "todo : " << range << " in Thread" <<  std::endl;
                        }
                        for(size_t i=range.latest;i>=range.offset;--i)
                        {
                            target[i] = source[i];
                        }
                    }

                };
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
                Parallel::Load<TARGET,SOURCE> code = {};
                carver.in1d(code,target,source);

            }
        }

    }

}

#endif

