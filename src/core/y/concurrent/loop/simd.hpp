
//! \file

#ifndef Y_Concurrent_SIMD_Included
#define Y_Concurrent_SIMD_Included 1

#include "y/concurrent/loop/interface.hpp"
#include "y/concurrent/engines.hpp"

namespace Yttrium
{
    namespace Concurrent
    {


        template <typename MAPPING>
        class SIMD : public Engines<MAPPING>
        {
        public:
            typedef Engines<MAPPING>               MyEngines;  //!< alias
            typedef typename MyEngines::EngineType EngineType; //!< alias
            typedef typename MyEngines::Propulsion Propulsion; //!< Writable engones

            inline virtual ~SIMD() noexcept {}

            inline SIMD(const SharedLoop &team) :
            MyEngines(team),
            loop(team)
            {
            }

            //! testing
            void operator()(void)  
            {
                const CallMe call = { *this };
                (*loop)(call);
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(SIMD);
            SharedLoop loop;

            struct CallMe
            {
                const Propulsion &engines;
                inline void operator()(const ThreadContext &ctx) const
                {
                    const EngineType &engine = engines[ctx.indx];
                    Y_LOCK(ctx.sync);
                    (std::cerr << "SIMD: in engine " << ctx.name << " : " << engine << std::endl).flush();
                }
            };

        };

        



    }

}

#endif
