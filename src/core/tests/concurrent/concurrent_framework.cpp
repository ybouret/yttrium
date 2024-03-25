#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"
#include "y/concurrent/pipeline/alone.hpp"
#include "y/concurrent/pipeline/queue.hpp"
#include "y/concurrent/split/for-loop.hpp"
#include "y/concurrent/split/tiling.hpp"

#include "y/concurrent/thread.hpp"
#include "y/string/env.hpp"

#include "y/memory/solitary/workspace.hpp"
#include "y/container/cxx/array.hpp"
#include "y/memory/allocator/dyadic.hpp"

#include "y/utest/run.hpp"


namespace Yttrium
{

    namespace Concurrent
    {

        namespace Nucleus
        {
            //! base class for all frame(s)
            class Frame : public ThreadContext
            {
            public:
                virtual ~Frame() noexcept {}

            protected:
                explicit Frame(const ThreadContext &ctx) noexcept : ThreadContext(ctx) {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Frame);
            };

            class  Punctual
            {
            public:
                explicit Punctual() noexcept {}
                virtual ~Punctual() noexcept {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Punctual);
            };
        }

        template <typename MAPPING>
        class Frame : public Nucleus::Frame
        {
        public:

            explicit Frame(const ThreadContext &ctx) noexcept :
            Nucleus::Frame(ctx),
            workspace()
            {
            }



        private:
            Memory::Workspace<MAPPING> workspace;
            Y_DISABLE_COPY_AND_ASSIGN(Frame);
        };

        class Frame0D : public Frame<Nucleus::Punctual>
        {
        public:
            typedef Nucleus::Punctual Mapping;
            typedef Frame<Mapping>    FrameType;

            virtual ~Frame0D() noexcept {}

        protected:
            explicit Frame0D(const ThreadContext &ctx) noexcept :
            FrameType(ctx) {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Frame0D);
        };

        template <typename T>
        class Frame1D : public Frame< ForLoop<T> >
        {
        public:
            typedef ForLoop<T>     Mapping;
            typedef Frame<Mapping> FrameType;

            virtual ~Frame1D() noexcept {}

        protected:
            explicit Frame1D(const ThreadContext &ctx) noexcept :
            FrameType(ctx)
            {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Frame1D);
        };

        template <typename T>
        class Frame2D : public Frame< Tiling<T> >
        {
        public:
            typedef Tiling<T>      Mapping;
            typedef Frame<Mapping> FrameType;

            virtual ~Frame2D() noexcept {}

        protected:
            explicit Frame2D(const ThreadContext &ctx) noexcept :
            FrameType(ctx)
            {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Frame2D);
        };


        namespace Nucleus
        {
            //__________________________________________________________________
            //
            //
            //
            //! use shared thread contexts to setup resources
            //
            //
            //__________________________________________________________________
            class Frames
            {
            public:
                typedef Memory::Dyadic Model; //!< Memory Model


                virtual ~Frames() noexcept {}

            protected:
                //! setup with pointer conversion
                /**
                 DERIVED can be Loop/Mono/Crew or Pipeline/Alone/Queue
                 */
                template <typename DERIVED>
                explicit Frames(const ArcPtr<DERIVED> &stc) noexcept :
                contexts(CopyOf,stc)
                {
                }

                //! local shared contexts
                ArcPtr<ThreadContexts>  contexts;



            private:
                Y_DISABLE_COPY_AND_ASSIGN(Frames);
            };
        }

        //! Frame[0|1|2]D
        template <typename FRAME>
        class Frames : 
        public Nucleus::Frames,
        public CxxArray<FRAME,Nucleus::Frames::Model>
        {
        public:
            typedef FRAME                                  FrameType;
            typedef typename FrameType::Mapping            Mapping;
            typedef CxxArray<FRAME,Nucleus::Frames::Model> CxxFrames;      //!< alias


            template <typename DERIVED>
            explicit Frames(const ArcPtr<DERIVED> &stc) :
            Nucleus::Frames(stc),
            CxxFrames(CopyOf,*contexts)
            {
            }

            
            virtual ~Frames() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Frames);
        };



    }
}

using namespace Yttrium;

namespace
{
    class Demo0D : public Concurrent::Frame0D
    {
    public:
        explicit Demo0D(const ThreadContext &ctx) noexcept: Concurrent::Frame0D(ctx)
        {
        }

        virtual ~Demo0D() noexcept
        {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Demo0D);
    };

    class Demo1D : public Concurrent::Frame1D<size_t>
    {
    public:

        explicit Demo1D(const ThreadContext &ctx) noexcept: Concurrent::Frame1D<size_t>(ctx)
        {
        }

        virtual ~Demo1D() noexcept
        {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Demo1D);
    };


    class Demo2D : public Concurrent::Frame2D<unit_t>
    {
    public:

        explicit Demo2D(const ThreadContext &ctx) noexcept: Concurrent::Frame2D<unit_t>(ctx)
        {
        }

        virtual ~Demo2D() noexcept
        {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Demo2D);
    };




}


Y_UTEST(concurrent_framework)
{
    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");
    const Concurrent::Topology topo;

    Concurrent::SharedLoop     seqLoop = new Concurrent::Mono();
    Concurrent::SharedLoop     parLoop = new Concurrent::Crew(topo);
    Concurrent::SharedPipeline seqPipe = new Concurrent::Alone();
    Concurrent::SharedPipeline parPipe = new Concurrent::Queue(topo);


    Concurrent::Frames<Demo0D> f0seq(seqPipe);
    Concurrent::Frames<Demo0D> f0par(parPipe);

    Concurrent::Frames<Demo1D> f1seq(seqLoop);
    Concurrent::Frames<Demo1D> f1par(parLoop);




}
Y_UDONE()

