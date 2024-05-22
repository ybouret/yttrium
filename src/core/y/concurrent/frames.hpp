
//! \file

#ifndef Y_Concurrent_Frames_Included
#define Y_Concurrent_Frames_Included 1

#include "y/concurrent/frame/nucleus/frames.hpp"
#include "y/container/cxx/array.hpp"
#include "y/mkl/v2d.hpp"
#include "y/memory/solitary/workspace.hpp"
#include "y/type/auto-clean.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        //______________________________________________________________________
        //
        //
        //! default frames constructor
        //
        //______________________________________________________________________
#define Y_Concurrent_Frames_Construct() \
Nucleus::Frames(stc),                   \
CxxFrames(CopyOf,static_cast<const ThreadContexts &>(*contexts)),            \
signature()



        //______________________________________________________________________
        //
        //
        //! helper to assign mapping to engines
        //
        //______________________________________________________________________
#define Y_Concurrent_Frames_Assign(CODE) do {                   \
/**/ if( signature.isValid() && (query == *signature) ) return; \
/**/ assert(signature.isEmpty() || (query != *signature) );     \
/**/ loosen();                                                  \
/**/ Writable<FRAME> &f = *this;                                \
/**/ const size_t     n = f.size();                             \
/**/ try{                                                       \
/**/      for(size_t i=1;i<=n;++i) { f[i].CODE;}                \
/**/      signature.build(query);                               \
/**/    }                                                       \
/**/ catch(...) { loosen(); throw; } } while(false)


        //______________________________________________________________________
        //
        //
        //! helper to execute CODE per FRAME
        //
        //______________________________________________________________________
#define Y_Concurrent_Frames_For_Each(CODE) do { \
/**/     assert(0!=meth);                       \
/**/     Writable<FRAME> &h = *this;            \
/**/     const size_t     n = h.size();         \
/**/     for(size_t i=1;i<=n;++i) { CODE; }     \
/**/ } while(false)


        //______________________________________________________________________
        //
        //
        //
        //! Precompiled/dynamic FRAME deriving from Frame[0|1|2]D
        /**
         - prepare one FRAME per context, a.k.a one FRAME(=resource) per thread
         - assign(FULL) => assign(PART) per context
         - loosen()     => loosen each FRAME
         */
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
            typedef V2D<typename FrameType::Type>          Vertex;    //!< in case of 2D
            typedef typename FRAME::Signature              Signature; //!< [0|1|2]D

            //__________________________________________________________________
            //
            //! automatic unlink resources
            //__________________________________________________________________
            class AutoUnlink : public AutoClean<Frames>
            {
            public:
                inline explicit AutoUnlink(Frames &frames) noexcept : AutoClean<Frames>(frames, & Frames::unlink) {} //!< setup
                inline virtual ~AutoUnlink()               noexcept {}                                               //!< cleanup
            private:
                Y_DISABLE_COPY_AND_ASSIGN(AutoUnlink);
            };

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! build for DERIVED from ThreadContexts
            /**
             - keep a shared copy of contexts
             - build one FRAME per context
             \param stc shared ThreadContexts (class DERIVED : public ThreadContexts...)
             */
            template <typename DERIVED> inline
            explicit Frames(const ArcPtr<DERIVED> &stc) :
            Y_Concurrent_Frames_Construct()
            {
            }


            //! cleanup
            inline virtual ~Frames() noexcept { loosen(); }

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! call a method for each FRAME (post-init)
            template <typename METH> inline
            void forEach(METH meth) { Y_Concurrent_Frames_For_Each((h[i].*meth)()); }

            //! call a method for each FRAME with arguments (post-init)
            template <typename METH, typename ARGS> inline
            void forEach(METH meth, ARGS &args) { Y_Concurrent_Frames_For_Each((h[i].*meth)(args)); }

            //__________________________________________________________________
            //
            //
            // extra constructors
            //
            //__________________________________________________________________

            //! helper to build frames + resource acquisition
            template <typename DERIVED, typename METH>
            explicit Frames(const ArcPtr<DERIVED> &stc, METH meth) :
            Y_Concurrent_Frames_Construct()
            {
                forEach<METH>(meth);
            }


            //! helper to build frames + resource acquisition
            template <typename DERIVED, typename METH, typename ARGS>
            explicit Frames(const ArcPtr<DERIVED> &stc, METH meth, ARGS &args) :
            Y_Concurrent_Frames_Construct()
            {
                forEach<METH>(meth,args);
            }



            //__________________________________________________________________
            //
            //! loosen all workspaces and erase signature
            //__________________________________________________________________
            inline void loosen() noexcept
            {
                signature.erase();
                Writable<FRAME> &self = *this;
                for(size_t i=self.size();i>0;--i)
                    self[i].loosen();
            }

            //__________________________________________________________________
            //
            //! assign to all PUNCTAL frames (deriving from Frame0D)
            //__________________________________________________________________
            inline void assign() {
                static const Signature query = 1;
                Y_Concurrent_Frames_Assign(assign());
            }

            //__________________________________________________________________
            //
            //! assign sub-ForLoop to all 1D frames
            //__________________________________________________________________
            inline void assign(const typename FrameType::Type head,
                               const typename FrameType::Type tail,
                               const typename FrameType::Type step)
            {
                const Signature query(head,tail,step);
                Y_Concurrent_Frames_Assign(assign(head,tail,step));
            }

            //__________________________________________________________________
            //
            //! attach sub-Tiles to all 2D frames: lower --> upper
            //__________________________________________________________________
            inline void assign(const V2D<typename FrameType::Type> lower,
                               const V2D<typename FrameType::Type> upper)
            {
                const Signature query(lower,upper);
                Y_Concurrent_Frames_Assign(assign(lower,upper));
            }

            //__________________________________________________________________
            //
            //! attach sub-Tiles to all 2D frames: (1,1) --> upper
            //__________________________________________________________________
            inline void assign(const V2D<typename FrameType::Type> upper)
            {
                const V2D<typename FrameType::Type> lower(1,1);
                assign(lower,upper);
            }



            //__________________________________________________________________
            //
            //
            // Helpers
            //
            //__________________________________________________________________
           
            //! propagate nodes if ENGINE has a link(NODE) method
            template <typename NODE> inline
            void link(NODE *node) noexcept
            {
                assert(0!=node);
                Writable<FRAME> &f = *this;
                const size_t     n = f.size();
                for(size_t i=1;i<=n;++i,node=node->next)
                {
                    assert(0!=node);
                    f[i].link(*node);
                }
            }

            //! propagate unlink() to ENGINE is it exists
            inline void unlink() noexcept
            {
                Writable<FRAME> &f = *this;
                for(size_t i=f.size();i>0;--i)
                    f[i].unlink();
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Frames);
            Memory::Workspace<Signature> signature;
        };

    }

}

#endif

