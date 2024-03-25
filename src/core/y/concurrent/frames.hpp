
//! \file

#ifndef Y_CONCURRENT_FRAMES_INCLUDED
#define Y_CONCURRENT_FRAMES_INCLUDED 1

#include "y/concurrent/frame/nucleus/frames.hpp"
#include "y/container/cxx/array.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
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

#endif

