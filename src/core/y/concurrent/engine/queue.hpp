
//! \file

#ifndef Y_Concurrent_QEngine_Included
#define Y_Concurrent_QEngine_Included 1

#include "y/concurrent/engine/interface.hpp"


namespace Yttrium
{
    namespace Concurrent
    {

        namespace Nucleus
        {
            class NoMapping
            {
            public:
                NoMapping() noexcept;
                ~NoMapping() noexcept;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(NoMapping);
            };
        };

        class QEngine : public Engine<Nucleus::NoMapping>
        {
        public:
            explicit QEngine() noexcept;
            virtual ~QEngine() noexcept;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(QEngine);
        };

    }

}

#endif
