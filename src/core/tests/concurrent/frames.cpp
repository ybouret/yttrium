#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"
#include "y/concurrent/pipeline/alone.hpp"
#include "y/concurrent/pipeline/queue.hpp"
#include "y/concurrent/split/for-loop.hpp"
#include "y/concurrent/split/tiling.hpp"

#include "y/concurrent/thread.hpp"
#include "y/string/env.hpp"

#include "y/concurrent/frame/interface.hpp"
#include "y/concurrent/frame/nucleus/punctual.hpp"

#include "y/container/cxx/array.hpp"
#include "y/memory/allocator/dyadic.hpp"

#include "y/utest/run.hpp"


namespace Yttrium
{

    namespace Concurrent
    {

        
        class Frame0D : public Frame<Nucleus::Punctual>
        {
        public:
            typedef int               Type;
            typedef Nucleus::Punctual Mapping;
            typedef Frame<Mapping>    FrameType;

            virtual ~Frame0D() noexcept {}

            void init()
            {
                this->workspace.build();
            }


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
            typedef T              Type;
            typedef ForLoop<T>     Mapping;
            typedef Frame<Mapping> FrameType;

            virtual ~Frame1D() noexcept {}

            void init(const T &head, const T &tail, const T &step)
            {
                const Mapping mapping = Split::For(*this, head, tail, step);
                this->workspace.build(mapping);
            }



        protected:
            explicit Frame1D(const ThreadContext &ctx) noexcept :
            FrameType(ctx)
            {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Frame1D);
        };

        template <typename T>
        class Frame2D : public Frame< AutoPtr< typename Tiling<T>::Tile > >
        {
        public:
            typedef T                        Type;
            typedef typename Tiling<T>::Tile Tile;
            typedef AutoPtr<Tile>            Mapping;
            typedef Frame<Mapping>           FrameType;

            virtual ~Frame2D() noexcept {}

            void init(const V2D<T> &lower, const V2D<T> &upper)
            {
                const Mapping mapping = Tiling<T>::Tiles::For(*this,lower,upper);
                this->workspace.build(mapping);
            }


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

            inline void quit() noexcept
            {
                Writable<FRAME> &self = *this;
                for(size_t i=self.size();i>0;--i)
                    self[i].quit();
            }

            inline void init()
            {
                quit();
                Writable<FRAME> &self = *this;
                const size_t     n    = self.size();
                try { for(size_t i=1;i<=n;++i) self[i].init(); }
                catch(...) { quit(); throw; }
            }

            inline void init(const typename FrameType::Type head,
                             const typename FrameType::Type tail,
                             const typename FrameType::Type step)
            {
                quit();
                Writable<FRAME> &self = *this;
                const size_t     n    = self.size();
                try { for(size_t i=1;i<=n;++i) self[i].init(head,tail,step); }
                catch(...) { quit(); throw; }
            }

            inline void init(const V2D<typename FrameType::Type> lower,
                             const V2D<typename FrameType::Type> upper)
            {
                quit();
                Writable<FRAME> &self = *this;
                const size_t     n    = self.size();
                try { for(size_t i=1;i<=n;++i) self[i].init(lower,upper); }
                catch(...) { quit(); throw; }
            }


            virtual ~Frames() noexcept { quit(); }

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


Y_UTEST(concurrent_frames)
{
    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");
    const Concurrent::Topology topo;

    Concurrent::SharedLoop     seqLoop = new Concurrent::Mono();
    Concurrent::SharedLoop     parLoop = new Concurrent::Crew(topo);
    Concurrent::SharedPipeline seqPipe = new Concurrent::Alone();
    Concurrent::SharedPipeline parPipe = new Concurrent::Queue(topo);

    {

        Concurrent::Frames<Demo0D> f0seq(seqPipe);
        Concurrent::Frames<Demo0D> f0par(parPipe);

        std::cerr << "f0seq=" << f0seq << std::endl;
        std::cerr << "f0par=" << f0par << std::endl;


        f0seq.init();
        f0par.init();

        std::cerr << "f0seq=" << f0seq << std::endl;
        std::cerr << "f0par=" << f0par << std::endl;

    }

    {
        Concurrent::Frames<Demo1D> f1seq(seqLoop);
        Concurrent::Frames<Demo1D> f1par(parLoop);

        std::cerr << "f1seq=" << f1seq << std::endl;
        std::cerr << "f1par=" << f1par << std::endl;


        f1seq.init(1,10,2);
        f1par.init(1,10,2);

        std::cerr << "f1seq=" << f1seq << std::endl;
        std::cerr << "f1par=" << f1par << std::endl;

    }

    {
        Concurrent::Frames<Demo2D> f2seq(seqLoop);
        Concurrent::Frames<Demo2D> f2par(parLoop);
        std::cerr << "f2seq=" << f2seq << std::endl;
        std::cerr << "f2par=" << f2par << std::endl;

        const V2D<unit_t> lower(1,1);
        const V2D<unit_t> upper(10,10);

        f2seq.init( lower, upper );
        f2par.init( lower, upper );

        std::cerr << "f2seq=" << f2seq << std::endl;
        std::cerr << "f2par=" << f2par << std::endl;
    }

}
Y_UDONE()

