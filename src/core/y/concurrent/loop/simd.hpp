
//! \file

#ifndef Y_Concurrent_SIMD_Included
#define Y_Concurrent_SIMD_Included 1

#include "y/concurrent/loop/interface.hpp"
#include "y/concurrent/resources.hpp"
#include "y/type/auto-clean.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        //______________________________________________________________________
        //
        //
        //
        //! SIMD for 0D/1D/2D
        /**
         Use Resources<> API to transform user's code into Kernel
         */
        //
        //
        //______________________________________________________________________
        template <typename RESOURCE>
        class SIMD : public Resources<RESOURCE>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Resources<RESOURCE>    MyResources;  //!< alias


            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! create resources according to team->size()
            inline explicit SIMD(const SharedLoop &team) :
            MyResources(team), loop( Coerce(*team) )
            {
            }

            //! cleanup
            inline virtual ~SIMD() noexcept {}

            //__________________________________________________________________
            //
            //
            // API
            //
            //__________________________________________________________________

            //! propagate attach(*node), node=node->next to each resource
            /**
             assuming that RESOURCE posesses a 'attach( *node )' method
             \param node first node of a least this->size nodes
             */
            template <typename NODE>
            inline void attach(NODE *node) noexcept
            {
                MyResources &self = *this;
                const size_t nres = self.size();
                for(size_t i=1;i<=nres;++i,node=node->next) {
                    assert(0!=node);
                    self[i].attach(*node);
                }
            }

            //! propagate detach() to each resource
            inline void detach() noexcept
            {
                MyResources &self = *this;
                for(size_t i=self.size();i>0;--i) self[i].detach();
            }

            class AutoDetach : public AutoClean
            {
            public:
                inline explicit AutoDetach(SIMD &simd) : AutoClean(simd, & SIMD::detach ) {}
                inline virtual ~AutoDetach() noexcept {}
            private:
                Y_DISABLE_COPY_AND_ASSIGN(AutoDetach);
            };

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //! testing
            //__________________________________________________________________
            inline void operator()(void)
            {
                const CallMe me = { *this };
                loop(me);
            }

            //__________________________________________________________________
            //
            //! no-arg: call proc(engine)
            //__________________________________________________________________
            template <typename PROC>
            inline void operator()(PROC &proc)
            {
                Call0<PROC> me = { *this, proc };
                loop(me);
            }

            //__________________________________________________________________
            //
            //! 1-arg: call proc(engine,arg1)
            //__________________________________________________________________
            template <typename PROC, typename ARG1>
            inline void operator()(PROC &proc, ARG1 &arg1)
            {
                Call1<PROC,ARG1> me = { *this, proc, arg1 };
                loop(me);
            }

            //__________________________________________________________________
            //
            //! 2-args: call proc(engine,arg1,arg2)
            //__________________________________________________________________
            template <typename PROC, typename ARG1, typename ARG2>
            inline void operator()(PROC &proc, ARG1 &arg1, ARG2 &arg2)
            {
                Call2<PROC,ARG1,ARG2> me = { *this, proc, arg1, arg2 };
                loop(me);
            }

            //__________________________________________________________________
            //
            //! 3-args: call proc(engine,arg1,arg2,arg3)
            //__________________________________________________________________
            template <typename PROC, typename ARG1, typename ARG2, typename ARG3>
            inline void operator()(PROC &proc, ARG1 &arg1, ARG2 &arg2, ARG3 &arg3)
            {
                Call3<PROC,ARG1,ARG2,ARG3> me = { *this, proc, arg1, arg2, arg3 };
                loop(me);
            }

            //__________________________________________________________________
            //
            //! 4-args: call proc(engine,arg1,arg2,arg3,arg4)
            //__________________________________________________________________
            template <typename PROC, typename ARG1, typename ARG2, typename ARG3, typename ARG4>
            inline void operator()(PROC &proc, ARG1 &arg1, ARG2 &arg2, ARG3 &arg3, ARG4 &arg4)
            {
                Call4<PROC,ARG1,ARG2,ARG3,ARG4> me = { *this, proc, arg1, arg2, arg3, arg4 };
                loop(me);
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(SIMD);
            Loop &loop;

            struct CallMe
            {
                const MyResources &self;
                inline void operator()(const ThreadContext &ctx) const
                {
                    const RESOURCE &resource = self[ctx.indx];
                    Y_LOCK(resource.sync);
                    (std::cerr << "SIMD: in resource: " << resource << std::endl).flush();
                }
            };


            template <typename PROC>
            struct Call0
            {
                MyResources &self;
                PROC        &proc;
                inline void operator()(const ThreadContext &ctx) const
                {
                    proc(self[ctx.indx]);
                }
            };

            template <typename PROC,typename ARG1>
            struct Call1
            {
                MyResources &self;
                PROC        &proc;
                ARG1        &arg1;
                inline void operator()(const ThreadContext &ctx) const
                {
                    proc(self[ctx.indx],arg1);
                }
            };

            template <typename PROC,typename ARG1, typename ARG2>
            struct Call2
            {
                MyResources &self;
                PROC        &proc;
                ARG1        &arg1;
                ARG2        &arg2;
                inline void operator()(const ThreadContext &ctx) const
                {
                    proc(self[ctx.indx],arg1,arg2);
                }
            };

            template <typename PROC,typename ARG1, typename ARG2, typename ARG3>
            struct Call3
            {
                MyResources &self;
                PROC        &proc;
                ARG1        &arg1;
                ARG2        &arg2;
                ARG3        &arg3;
                inline void operator()(const ThreadContext &ctx) const
                {
                    proc(self[ctx.indx],arg1,arg2,arg3);
                }
            };

            template <typename PROC,typename ARG1, typename ARG2, typename ARG3, typename ARG4>
            struct Call4
            {
                MyResources &self;
                PROC        &proc;
                ARG1        &arg1;
                ARG2        &arg2;
                ARG3        &arg3;
                ARG4        &arg4;
                inline void operator()(const ThreadContext &ctx) const
                {
                    proc(self[ctx.indx],arg1,arg2,arg3,arg4);
                }
            };



        };

        


    }

}

#endif
