


//! \file

#ifndef Y_Concurrent_Resource1D_Included
#define Y_Concurrent_Resource1D_Included 1

#include "y/concurrent/resource/interface.hpp"
#include "y/concurrent/split/for-loop.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        template <typename T>
        class Resource1D : public Resource< ForLoop<T> >
        {
        public:
            typedef Resource< ForLoop<T> >     BaseType;
            typedef typename BaseType::Mapping Mapping;
            typedef typename Mapping::Type     Type;
            typedef typename Mapping::Size     Size;

            inline virtual ~Resource1D() noexcept {}

        protected:
            inline explicit Resource1D(const ThreadContext &ctx) : BaseType(ctx) {}
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Resource1D);
        };

    }

}

#endif
