//! \file

#ifndef Y_Concurrent_Engine_Included
#define Y_Concurrent_Engine_Included 1

#include "y/concurrent/thread/context.hpp"


namespace Yttrium
{
    namespace Concurrent
    {
     
        namespace Nucleus
        {
            class Engine
            {
            public:
                virtual     ~Engine() noexcept;
            protected:
                explicit     Engine() noexcept;
                virtual void activate(const ThreadContext &) = 0;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Engine);
            };
        }

        template <typename MAPPING>
        class Engine : public Nucleus::Engine
        {
        public:
            typedef MAPPING Mapping;



            inline friend std::ostream & operator<<(std::ostream &os, const Engine &self)
            {
                return os << self.mapping;
            }

            Mapping mapping;

            inline virtual ~Engine() noexcept {}
        protected:
            //! default setup, mapping is empty
            inline explicit Engine() noexcept : mapping() {}

            //!
            inline void initiate(const ThreadContext &cntx, Mapping &temp)
            {
                mapping.xch(temp);
                activate(cntx);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Engine);
        };



    }

}

#endif

