
//! \file


#ifndef Y_Concurrent_SIMT_Included
#define Y_Concurrent_SIMT_Included 1

#include "y/concurrent/loop/simt/mill.hpp"
#include "y/concurrent/frames.hpp"

namespace Yttrium
{
    namespace Concurrent
    {


        //______________________________________________________________________
        //
        //
        //
        //! Frames+Loop : Single Instuction Multiple Threads
        //
        //
        //______________________________________________________________________
        template <typename ENGINE>
        class SIMT : public SIMT_Mill, public Frames<ENGINE>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definition
            //
            //__________________________________________________________________
            typedef Writable<ENGINE> FramesType; //!< alias


            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup
            /**
             ARGS may be:
             - SharedLoop for a manual setup
             - Sequential_ to use Mono()
             - InParallel_ to use Crew::Create()
             */
            template <typename ARGS>
            inline explicit  SIMT( const ARGS &args ) :
            SIMT_Mill(args), Frames<ENGINE>(mill)
            {
            }



            //! setup with postInit method
            template <typename ARGS, typename METH>
            inline explicit SIMT(const ARGS &args, METH meth) :
            SIMT_Mill(args), Frames<ENGINE>(mill,meth)
            {
            }



            //! cleanup
            inline virtual ~SIMT() noexcept {}

            //__________________________________________________________________
            //
            //! Testing
            //__________________________________________________________________
            inline void operator()(void) {
                CallMe me = { *this };
                run(me);
            }

            //__________________________________________________________________
            //
            //! no-arg: call proc(engine)
            //__________________________________________________________________
            template <typename PROC>
            inline void operator()(PROC &proc)
            {
                Call0<PROC> me = { *this, proc };
                run(me);
            }


            //__________________________________________________________________
            //
            //! 1-arg: call proc(engine,arg1)
            //__________________________________________________________________
            template <typename PROC, typename ARG1>
            inline void operator()(PROC &proc, ARG1 &arg1)
            {
                Call1<PROC,ARG1> me = { *this, proc, arg1 };
                run(me);
            }

            //__________________________________________________________________
            //
            //! 2-args: call proc(engine,arg1,arg2)
            //__________________________________________________________________
            template <typename PROC, typename ARG1, typename ARG2>
            inline void operator()(PROC &proc, ARG1 &arg1, ARG2 &arg2)
            {
                Call2<PROC,ARG1,ARG2> me = { *this, proc, arg1, arg2 };
                run(me);
            }

            //__________________________________________________________________
            //
            //! 3-args: call proc(engine,arg1,arg2,arg3)
            //__________________________________________________________________
            template <typename PROC, typename ARG1, typename ARG2, typename ARG3>
            inline void operator()(PROC &proc, ARG1 &arg1, ARG2 &arg2, ARG3 &arg3)
            {
                Call3<PROC,ARG1,ARG2,ARG3> me = { *this, proc, arg1, arg2, arg3 };
                run(me);
            }

            //__________________________________________________________________
            //
            //! 4-args: call proc(engine,arg1,arg2,arg3,arg4)
            //__________________________________________________________________
            template <typename PROC, typename ARG1, typename ARG2, typename ARG3, typename ARG4>
            inline void operator()(PROC &proc, ARG1 &arg1, ARG2 &arg2, ARG3 &arg3, ARG4 &arg4)
            {
                Call4<PROC,ARG1,ARG2,ARG3,ARG4> me = { *this, proc, arg1, arg2, arg3, arg4 };
                run(me);
            }

            //__________________________________________________________________
            //
            //! 5-args: call proc(engine,arg1,arg2,arg3,arg4,arg5)
            //__________________________________________________________________
            template <typename PROC, typename ARG1, typename ARG2, typename ARG3, typename ARG4, typename ARG5>
            inline void operator()(PROC &proc, ARG1 &arg1, ARG2 &arg2, ARG3 &arg3, ARG4 &arg4, ARG5 &arg5)
            {
                Call5<PROC,ARG1,ARG2,ARG3,ARG4,ARG5> me = { *this, proc, arg1, arg2, arg3, arg4, arg5 };
                run(me);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(SIMT);

            struct CallMe
            {
                FramesType &self;
                inline void operator()(const ThreadContext &ctx)
                {
                    Y_LOCK(ctx.sync);
                    ENGINE &host = self[ctx.indx];
                    (std::cerr << "call SIMT() from " << host.name <<std::endl).flush();
                }
            };

            template <typename PROC>
            struct Call0
            {
                FramesType &self;
                PROC       &proc;
                inline void operator()(const ThreadContext &ctx) {
                    proc(self[ctx.indx]);
                }
            };

            template <typename PROC,typename ARG1>
            struct Call1
            {
                FramesType &self;
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
                FramesType &self;
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
                FramesType &self;
                PROC       &proc;
                ARG1       &arg1;
                ARG2       &arg2;
                ARG3       &arg3;
                inline void operator()(const ThreadContext &ctx) const
                {
                    proc(self[ctx.indx],arg1,arg2,arg3);
                }
            };

            template <typename PROC,typename ARG1, typename ARG2, typename ARG3, typename ARG4>
            struct Call4
            {
                FramesType &self;
                PROC       &proc;
                ARG1       &arg1;
                ARG2       &arg2;
                ARG3       &arg3;
                ARG4       &arg4;
                inline void operator()(const ThreadContext &ctx) const
                {
                    proc(self[ctx.indx],arg1,arg2,arg3,arg4);
                }
            };

            template <typename PROC,typename ARG1, typename ARG2, typename ARG3, typename ARG4, typename ARG5>
            struct Call5
            {
                FramesType &self;
                PROC       &proc;
                ARG1       &arg1;
                ARG2       &arg2;
                ARG3       &arg3;
                ARG4       &arg4;
                ARG5       &arg5;
                inline void operator()(const ThreadContext &ctx) const
                {
                    proc(self[ctx.indx],arg1,arg2,arg3,arg4,arg5);
                }
            };
        };
    }

}

#endif

