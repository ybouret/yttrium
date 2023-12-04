
//! \file

#ifndef Y_Concurrent_Engine1D_Included
#define Y_Concurrent_Engine1D_Included 1

#include "y/concurrent/engine/interface.hpp"
#include "y/concurrent/split/for-loop.hpp"


namespace Yttrium
{
    namespace Concurrent
    {

        template <typename T>
        class Engine1D : public Engine< ForLoop<T> >
        {
        public:
            typedef ForLoop<T>       Mapping;
            typedef Engine<Mapping>  EngineType;

            inline virtual ~Engine1D() noexcept {}

            inline void start(const ThreadContext &cntx, const T head, const T tail, const T step)
            {
                Mapping temp = Split::For(cntx,head,tail,step);
                this->initiate(cntx,temp);
            }

        protected:
            inline explicit Engine1D() noexcept : EngineType() {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Engine1D);
        };
    }

}

#endif

