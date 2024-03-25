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

                virtual void activate()          = 0; //!< post-init
                virtual void shutdown() noexcept = 0; //!< pre-quit

            protected:
                explicit Frame(const ThreadContext &ctx) noexcept : ThreadContext(ctx) 
                {
                    std::cerr << "New Frame@" << name << std::endl;
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Frame);
            };

            class  Punctual
            {
            public:
                typedef int Type;
                explicit Punctual() noexcept {}
                virtual ~Punctual() noexcept {}

                friend std::ostream & operator<<(std::ostream &os, const Punctual &)
                {
                    os << "(punctual)";
                    return os;
                }

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

            virtual ~Frame() noexcept
            {
                quit();
            }

            inline void quit() noexcept
            {
                if(this->workspace.isValid())
                {
                    this->shutdown();
                    this->workspace.dismiss();
                }
            }

            inline friend std::ostream & operator<<(std::ostream &os, const Frame &frame)
            {
                os << "@" << frame.name << "->" << frame.workspace;
                return os;
            }

        protected:
            Memory::Workspace<MAPPING> workspace;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Frame);
        };

        class Frame0D : public Frame<Nucleus::Punctual>
        {
        public:
            typedef Nucleus::Punctual Mapping;
            typedef Frame<Mapping>    FrameType;

            virtual ~Frame0D() noexcept {}

            void init()
            {
                assert(workspace.isEmpty());
                this->workspace.build();
                this->activate();
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
            typedef ForLoop<T>     Mapping;
            typedef Frame<Mapping> FrameType;

            virtual ~Frame1D() noexcept {}

            void init(const T head, const T tail, const T step)
            {
                assert(this->workspace.isEmpty());
                const Mapping mapping = Split::For(*this, head, tail, step);
                this->workspace.build(mapping);
                this->activate();
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
            typedef typename Tiling<T>::Tile Tile;
            typedef AutoPtr<Tile>  Mapping;
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

            inline void init(const typename Mapping::Type head,
                             const typename Mapping::Type tail,
                             const typename Mapping::Type step)
            {
                quit();
                Writable<FRAME> &self = *this;
                const size_t     n    = self.size();
                try { for(size_t i=1;i<=n;++i) self[i].init(head,tail,step); }
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

        virtual void activate()
        {
            std::cerr << "Demo0D activate@" << name << std::endl;
        }

        virtual void shutdown() noexcept
        {
            std::cerr << "Demo0D shutdown@" << name << std::endl;
        }

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
        virtual void activate()
        {
            std::cerr << "Demo1D activate" << std::endl;
        }

        virtual void shutdown() noexcept
        {
            std::cerr << "Demo1D shutdown" << std::endl;
        }
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
        virtual void activate()
        {
            std::cerr << "Demo2D activate" << std::endl;
        }

        virtual void shutdown() noexcept
        {
            std::cerr << "Demo2D shutdown" << std::endl;
        }
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


    Concurrent::Frames<Demo0D> f0seq(seqPipe);
    Concurrent::Frames<Demo0D> f0par(parPipe);

    std::cerr << "f0seq=" << f0seq << std::endl;
    std::cerr << "f0par=" << f0par << std::endl;


    f0seq.init();
    f0par.init();

    std::cerr << "f0seq=" << f0seq << std::endl;
    std::cerr << "f0par=" << f0par << std::endl;



    Concurrent::Frames<Demo1D> f1seq(seqLoop);
    Concurrent::Frames<Demo1D> f1par(parLoop);

    std::cerr << "f1seq=" << f1seq << std::endl;
    std::cerr << "f1par=" << f1par << std::endl;


    f1seq.init(1,10,2);
    f1par.init(1,10,2);
    
    std::cerr << "f1seq=" << f1seq << std::endl;
    std::cerr << "f1par=" << f1par << std::endl;


    Concurrent::Frames<Demo2D> f2seq(seqLoop);
    Concurrent::Frames<Demo2D> f2par(parLoop);


}
Y_UDONE()

