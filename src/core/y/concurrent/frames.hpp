
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

#define Y_Concurrent_Frames_Assign(CODE) do {   \
/**/ loosen();                                  \
/**/ Writable<FRAME> &f = *this;                \
/**/ const size_t     n = f.size();             \
/**/ try{ for(size_t i=1;i<=n;++i) f[i].CODE ;} \
/**/ catch(...) { loosen(); throw; } } while(false)

        //______________________________________________________________________
        //
        //
        //
        //! Precompiled/dynamic FRAME deriving from Frame[0|1|2]D
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
            /**
             - keep a shared copy of contexts
             - build one FRAME per context
             \param stc shared ThreadContexts (class DERIVED : public ThreadContexts...)
             */
            template <typename DERIVED>
            explicit Frames(const ArcPtr<DERIVED> &stc) :
            Nucleus::Frames(stc), CxxFrames(CopyOf,*contexts)
            {
            }


            //! cleanup
            virtual ~Frames() noexcept { loosen(); }

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________


            //__________________________________________________________________
            //
            //! loosen all workspaces
            //__________________________________________________________________
            inline void loosen() noexcept
            {
                Writable<FRAME> &self = *this;
                for(size_t i=self.size();i>0;--i)
                    self[i].loosen();
            }

            //__________________________________________________________________
            //
            //! assign to all PUNCTAL frames (deriving from Frame0D)
            //__________________________________________________________________
            inline void assign() { Y_Concurrent_Frames_Assign(assign()); }

            //__________________________________________________________________
            //
            //! assign sub-ForLoop to all 1D frames
            //__________________________________________________________________
            inline void assign(const typename FrameType::Type head,
                               const typename FrameType::Type tail,
                               const typename FrameType::Type step)
            {
                Y_Concurrent_Frames_Assign(assign(head,tail,step));
            }

            //__________________________________________________________________
            //
            //! attach sub-Tiles to all 2D frames
            //__________________________________________________________________
            inline void assign(const V2D<typename FrameType::Type> lower,
                               const V2D<typename FrameType::Type> upper)
            {
                Y_Concurrent_Frames_Assign(assign(lower,upper));
            }

            //__________________________________________________________________
            //
            //
            // Helpers
            //
            //__________________________________________________________________
            template <typename NODE> inline
            void link(NODE *node) noexcept
            {
                assert(0!=node);
                Writable<FRAME> &f = *this;
                const size_t     n = f.size();
                for(size_t i=1;i<=n;++i,node=node->next)
                {
                    assert(0!=node);
                    f[i].link(node);
                }
            }




        private:
            Y_DISABLE_COPY_AND_ASSIGN(Frames);
        };

    }

}

#endif

