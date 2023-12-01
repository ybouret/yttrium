
//! \file

#ifndef Y_Concurrent_SIMD_Included
#define Y_Concurrent_SIMD_Included 1

#include "y/concurrent/loop/interface.hpp"
#include "y/concurrent/split/for-loop.hpp"
#include "y/ostream-proto.hpp"
#include "y/container/cxx/array.hpp"

namespace Yttrium
{
    namespace Concurrent
    {


        namespace Nucleus
        {
            //! base class to handle Shared Loop
            class SIMD
            {
            public:
                virtual ~SIMD() noexcept;

            protected:
                explicit SIMD(const SharedLoop &team) noexcept;
                explicit SIMD(Loop             *team) noexcept;

                SharedLoop loop;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(SIMD);
            };
        }

#if 0
        template <typename T>
        class SIMD : public Nucleus::SIMD, public CxxArray< const ForLoop<T> >
        {
        public:
            typedef CxxArray< const ForLoop<T> > Ranges;

            inline explicit SIMD(const SharedLoop &team) :
            Nucleus::SIMD(team), Ranges(loop->size())
            {
            }

            inline explicit SIMD(Loop *team) :
            Nucleus::SIMD(team), Ranges(loop->size())
            {
            }

            inline void dispatch(const T head, const T tail, const T step)
            {
                const Readable<const ThreadContext> &cntx = *loop;
                Writable<const ForLoop<T> >         &self = *this; assert(self.size()==cntx.size());
                for(size_t i=cntx.size();i>0;--i)
                {
                    ForLoop<T> trek = Split::For(cntx[i],head,tail,step);
                    Coerce(self[i]).swapWith(trek);
                }
            }

            inline void operator()(void) {
                const CallMe call = { *this };
                (*loop)(call);
            }





        private:
            Y_DISABLE_COPY_AND_ASSIGN(SIMD);

            struct CallMe
            {
                const Ranges &self;
                void operator()(const ThreadContext &ctx) const
                {
                    const ForLoop<T> &trek = self[ctx.indx];
                    Y_LOCK(ctx.sync);
                    std::cerr << "SIMD @" << trek << std::endl;
                }
            };


        };
#endif
 

    }

}

#endif
