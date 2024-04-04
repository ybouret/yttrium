//! \file

#ifndef Y_CONCURRENT_FRAME1D_INCLUDED
#define Y_CONCURRENT_FRAME1D_INCLUDED 1

#include "y/concurrent/split/for-loop.hpp"
#include "y/concurrent/frame/interface.hpp"
#include "y/ptr/bare.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //
        //! Frame for 1D operations
        //
        //
        //______________________________________________________________________
        template <typename T>
        class Frame1D : public Frame< ForLoop<T> >
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef T                      Type;       //!< alias
            typedef ForLoop<T>             Mapping;    //!< alias
            typedef Frame<Mapping>         FrameType;  //!< alias
            typedef BarePtr<const Mapping> LoopPtr;    //!< alias>

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            
            //! assign partial ForLoop
            /**
             - compute partial ForLoop for context
             - assign loop if resulting length is positive
             */
            inline void assign(const T &head, const T &tail, const T &step)
            {
                assert(0==loop);
                const Mapping  part = Split::For(*this, head, tail, step);
                const Mapping &here = this->workspace.build(part);
                if(here.length>0)
                    Coerce(loop) = &here;
            }

            //! access sub ForLoop
            inline const ForLoop<T> & operator*() const noexcept
            {
                assert(this->workspace.isValid());
                return *(this->workspace);
            }


            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! cleanup
            inline virtual ~Frame1D() noexcept {}

        protected:
            //! setup
            inline explicit Frame1D(const ThreadContext &ctx) noexcept :
            FrameType(ctx), loop(0)
            {
            }


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            //! partial ACTIVE loop
            const ForLoop<T> * const loop;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Frame1D);
            inline virtual void shutdown() noexcept { Coerce(loop) = 0; }
        };
    }

}

#endif

