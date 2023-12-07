
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

            namespace Bof
            {
                template <typename TARGET, typename SOURCE>
                struct Todo
                {
                    inline void operator()(Motor1D &range,
                                           TARGET  &target,
                                           SOURCE  &source)
                    {

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
                Bof::Todo<TARGET,SOURCE> todo = {};
                carver.in1d.call(todo,target,source);

#if 0
                carver.setup(target.size());
                struct Todo {
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
                Todo todo = {};
                Carver::Proc1D &proc = carver.in1d;
                proc.template call<Todo,TARGET,SOURCE>(todo,target,source);
#endif
            }
        }

    }

}

#endif

