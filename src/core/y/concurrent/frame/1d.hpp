//! \file

#ifndef Y_CONCURRENT_FRAME1D_INCLUDED
#define Y_CONCURRENT_FRAME1D_INCLUDED 1

#include "y/concurrent/split/for-loop.hpp"
#include "y/concurrent/frame/interface.hpp"

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
            typedef T              Type;       //!< alias
            typedef ForLoop<T>     Mapping;    //!< alias
            typedef Frame<Mapping> FrameType;  //!< alias


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! assign sub ForLoop
            inline void assign(const T &head, const T &tail, const T &step)
            {
                const Mapping mapping = Split::For(*this, head, tail, step);
                this->workspace.build(mapping);
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
            FrameType(ctx)
            {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Frame1D);

        };
    }

}

#endif

