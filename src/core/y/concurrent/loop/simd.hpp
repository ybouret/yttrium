
//! \file

#ifndef Y_Concurrent_SIMD_Included
#define Y_Concurrent_SIMD_Included 1

#include "y/concurrent/loop/interface.hpp"
#include "y/concurrent/engines.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        //______________________________________________________________________
        //
        //
        //
        //! SIMD for 1D/2D
        //
        //
        //______________________________________________________________________
        template <typename MAPPING>
        class SIMD : public Engines<MAPPING>
        {
        public:
            typedef Engines<MAPPING>               MyEngines;  //!< alias
            typedef typename MyEngines::EngineType EngineType; //!< alias Engine[1|2]D
            typedef typename MyEngines::Propulsion Propulsion; //!< Writable engines to access resources

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

            //! no-arg call proc(engine)
            template <typename PROC>
            inline void operator()(PROC &proc)
            {
                Call0<PROC> call = { *this, proc };
                (*loop)(call);
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(SIMD);
            SharedLoop loop;

            struct CallMe
            {
                const Propulsion &self;
                inline void operator()(const ThreadContext &ctx) const
                {
                    const EngineType &engine = self[ctx.indx];
                    Y_LOCK(ctx.sync);
                    (std::cerr << "SIMD: in engine " << ctx.name << " : " << engine << std::endl).flush();
                }
            };

            template <typename PROC>
            struct Call0
            {
                Propulsion &self;
                PROC       &proc;
                inline void operator()(const ThreadContext &ctx) const
                {
                    proc(self[ctx.indx]);
                }
            };

        };

        



    }

}

#endif
