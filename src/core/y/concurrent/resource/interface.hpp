
//! \file

#ifndef Y_Concurrent_Resource_Included
#define Y_Concurrent_Resource_Included 1

#include "y/concurrent/thread/context.hpp"
#include <iostream>

namespace Yttrium
{
    namespace Concurrent
    {

        namespace Nucleus
        {
            class Resource : public ThreadContext
            {
            public:
                virtual ~Resource() noexcept;

            protected:
                explicit Resource(const ThreadContext &) noexcept;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Resource);
            };
        }

        

        //! Resource to map space into sub-parts
        template <typename MAPPING>
        class Resource : public Nucleus::Resource, public MAPPING
        {
        public:
            typedef MAPPING Mapping; //!< ForLoop<>, Tile,

            inline friend std::ostream & operator<<(std::ostream &os, const Resource &self) {
                return os << "@" << self.name << ":" << static_cast<const Mapping &>(self);
            }

            inline virtual ~Resource() noexcept {}

        protected:
            //! assign resource to context
            inline explicit Resource(const ThreadContext &ctx) :
            Nucleus::Resource(ctx),
            Mapping()
            {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Resource);
        };



    }

}

#endif
