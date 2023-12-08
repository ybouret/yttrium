
//! \file

#ifndef Y_Concurrent_SIMD_Included
#define Y_Concurrent_SIMD_Included 1

#include "y/concurrent/loop/interface.hpp"
#include "y/concurrent/resources.hpp"

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
            typedef RESOURCE               ResourceType;   //!< alias
            typedef Resources<RESOURCE>    ResourcesType;  //!< alias
            typedef Writable<ResourceType> Ranges;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________


            //! create default engines according to team->size()
            inline explicit SIMD(const SharedLoop &team) :
            ResourcesType(team), loop( Coerce(*team) )
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

            //! testing
            inline void operator()(void)
            {
                const CallMe me = { *this };
                loop(me);
            }


            //! no-arg call proc(engine)
            template <typename PROC>
            inline void operator()(PROC &proc)
            {
                Call0<PROC> me = { *this, proc };
                loop(me);
            }

            //! 1-arg call proc(engine,arg1)
            template <typename PROC, typename ARG1>
            inline void operator()(PROC &proc, ARG1 &arg1)
            {
                Call1<PROC,ARG1> me = { *this, proc, arg1 };
                loop(me);
            }

            //! 2-args call proc(engine,arg1,arg2)
            template <typename PROC, typename ARG1, typename ARG2>
            inline void operator()(PROC &proc, ARG1 &arg1, ARG2 &arg2)
            {
                Call2<PROC,ARG1,ARG2> me = { *this, proc, arg1, arg2 };
                loop(me);
            }

            //! 3-args call proc(engine,arg1,arg2,arg3)
            template <typename PROC, typename ARG1, typename ARG2, typename ARG3>
            inline void operator()(PROC &proc, ARG1 &arg1, ARG2 &arg2, ARG3 &arg3)
            {
                Call3<PROC,ARG1,ARG2,ARG3> me = { *this, proc, arg1, arg2, arg3 };
                loop(me);
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(SIMD);
            Loop &loop;

            struct CallMe
            {
                const ResourcesType &self;
                inline void operator()(const ThreadContext &ctx) const
                {
                    const ResourceType &resource = self[ctx.indx];
                    Y_LOCK(resource.sync);
                    (std::cerr << "SIMD: in resource: " << resource << std::endl).flush();
                }
            };


            template <typename PROC>
            struct Call0
            {
                ResourcesType &self;
                PROC          &proc;
                inline void operator()(const ThreadContext &ctx) const
                {
                    proc(self[ctx.indx]);
                }
            };

            template <typename PROC,typename ARG1>
            struct Call1
            {
                ResourcesType &self;
                PROC          &proc;
                ARG1          &arg1;
                inline void operator()(const ThreadContext &ctx) const
                {
                    proc(self[ctx.indx],arg1);
                }
            };

            template <typename PROC,typename ARG1, typename ARG2>
            struct Call2
            {
                ResourcesType &self;
                PROC          &proc;
                ARG1          &arg1;
                ARG2          &arg2;
                inline void operator()(const ThreadContext &ctx) const
                {
                    proc(self[ctx.indx],arg1,arg2);
                }
            };

            template <typename PROC,typename ARG1, typename ARG2, typename ARG3>
            struct Call3
            {
                ResourcesType &self;
                PROC          &proc;
                ARG1          &arg1;
                ARG2          &arg2;
                ARG3          &arg3;
                inline void operator()(const ThreadContext &ctx) const
                {
                    proc(self[ctx.indx],arg1,arg2,arg3);
                }
            };



        };

        


    }

}

#endif
