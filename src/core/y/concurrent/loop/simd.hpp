
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
        /**
         Execute samed code for different user defined resources
         */
        //
        //
        //______________________________________________________________________
        template <typename MAPPING>
        class SIMD : public Engines<MAPPING>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Engines<MAPPING>               MyEngines;  //!< alias
            typedef typename MyEngines::EngineType EngineType; //!< alias Engine[1|2]D
            typedef typename MyEngines::Propulsion Propulsion; //!< Writable engines to access resources

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________


            //! create default engines according to team->size()
            inline explicit SIMD(const SharedLoop &team) :
            MyEngines(team), loop( Coerce(*team) )
            {
            }

            //! cleanup
            inline virtual ~SIMD() noexcept {}

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

#if 0
            //! testing
            inline void operator()(void)
            {
                const CallMe call = { *this };
                loop(call);
            }
#endif
            
            //! no-arg call proc(engine)
            template <typename PROC>
            inline void operator()(PROC &proc)
            {
                Call0<PROC> call = { *this, proc };
                loop(call);
            }

            //! 1-arg call proc(engine,arg1)
            template <typename PROC, typename ARG1>
            inline void operator()(PROC &proc, ARG1 &arg1)
            {
                Call1<PROC,ARG1> call = { *this, proc, arg1 };
                loop(call);
            }

            //! 2-args call proc(engine,arg1,arg2)
            template <typename PROC, typename ARG1, typename ARG2>
            inline void operator()(PROC &proc, ARG1 &arg1, ARG2 &arg2)
            {
                Call2<PROC,ARG1,ARG2> call = { *this, proc, arg1, arg2 };
                loop(call);
            }

            //! 3-args call proc(engine,arg1,arg2,arg3)
            template <typename PROC, typename ARG1, typename ARG2, typename ARG3>
            inline void operator()(PROC &proc, ARG1 &arg1, ARG2 &arg2, ARG3 &arg3)
            {
                Call3<PROC,ARG1,ARG2,ARG3> call = { *this, proc, arg1, arg2, arg3 };
                loop(call);
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(SIMD);
            Loop &loop;

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

            template <typename PROC,typename ARG1>
            struct Call1
            {
                Propulsion &self;
                PROC       &proc;
                ARG1       &arg1;
                inline void operator()(const ThreadContext &ctx) const
                {
                    proc(self[ctx.indx],arg1);
                }
            };

            template <typename PROC,typename ARG1, typename ARG2>
            struct Call2
            {
                Propulsion &self;
                PROC       &proc;
                ARG1       &arg1;
                ARG2       &arg2;
                inline void operator()(const ThreadContext &ctx) const
                {
                    proc(self[ctx.indx],arg1,arg2);
                }
            };

            template <typename PROC,typename ARG1, typename ARG2, typename ARG3>
            struct Call3
            {
                Propulsion &self;
                PROC       &proc;
                ARG1       &arg1;
                ARG2       &arg2;
                ARG3       &arg3;
                inline void operator()(const ThreadContext &ctx) const
                {
                    proc(self[ctx.indx],arg1,arg2,arg3);
                }
            };




        };

        



    }

}

#endif
