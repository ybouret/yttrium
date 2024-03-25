//! \file

#ifndef Y_CONCURRENT_FRAME1D_INCLUDED
#define Y_CONCURRENT_FRAME1D_INCLUDED 1

#include "y/concurrent/split/for-loop.hpp"
#include "y/concurrent/frame/interface.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
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
    }

}

#endif

