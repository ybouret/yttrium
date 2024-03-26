
//! \file

#ifndef Y_CONCURRENT_FRAMES_INCLUDED
#define Y_CONCURRENT_FRAMES_INCLUDED 1

#include "y/concurrent/frame/nucleus/frames.hpp"
#include "y/container/cxx/array.hpp"
#include "y/mkl/v2d.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //
        //! Precompiled/dynamic Frame[0|1|2]D
        //
        //
        //______________________________________________________________________
        template <typename FRAME>
        class Frames : public Nucleus::Frames, public CxxArray<FRAME,Nucleus::Frames::Model>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef FRAME                                  FrameType; //!< alias
            typedef typename FrameType::Mapping            Mapping;   //!< alias
            typedef CxxArray<FRAME,Nucleus::Frames::Model> CxxFrames; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! DERIVED from ThreadContexts
            template <typename DERIVED>
            explicit Frames(const ArcPtr<DERIVED> &stc) :
            Nucleus::Frames(stc), CxxFrames(CopyOf,*contexts)
            {
            }


            //! cleanup
            virtual ~Frames() noexcept { detach(); }

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //! empty all workspaces
            //__________________________________________________________________
            inline void detach() noexcept
            {
                Writable<FRAME> &self = *this;
                for(size_t i=self.size();i>0;--i)
                    self[i].detach();
            }

            //__________________________________________________________________
            //
            //! setup to all punctual frames
            //__________________________________________________________________
            inline void attach()
            {
                detach();
                Writable<FRAME> &self = *this;
                const size_t     n    = self.size();
                try { for(size_t i=1;i<=n;++i) self[i].attach(); }
                catch(...) { detach(); throw; }
            }

            //__________________________________________________________________
            //
            //! attach sub-ForLoop to all 1D frames
            //__________________________________________________________________
            inline void attach(const typename FrameType::Type head,
                               const typename FrameType::Type tail,
                               const typename FrameType::Type step)
            {
                detach();
                Writable<FRAME> &self = *this;
                const size_t     n    = self.size();
                try { for(size_t i=1;i<=n;++i) self[i].attach(head,tail,step); }
                catch(...) { detach(); throw; }
            }

            //__________________________________________________________________
            //
            //! attach sub-Tiles to all 2D frames
            //__________________________________________________________________
            inline void attach(const V2D<typename FrameType::Type> lower,
                               const V2D<typename FrameType::Type> upper)
            {
                detach();
                Writable<FRAME> &self = *this;
                const size_t     n    = self.size();
                try { for(size_t i=1;i<=n;++i) self[i].attach(lower,upper); }
                catch(...) { detach(); throw; }
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Frames);
        };

    }

}

#endif

