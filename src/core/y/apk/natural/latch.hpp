

//! \file

#ifndef Y_APK_Natural_Latch_Included
#define Y_APK_Natural_Latch_Included 1

#include "y/object.hpp"
#include "y/data/pool/cxx.hpp"
#include "y/concurrent/mutex.hpp"
#include "y/ptr/ptr.hpp"

namespace Yttrium
{
    namespace APK
    {
        class Latch : public Object, public Concurrent::Mutex
        {
        public:
            typedef CxxPoolOf<Latch> Pool;
            typedef Ptr<Latch,Immediate> PtrType;
            class Handle : public PtrType
            {
            public:
                explicit Handle();
                virtual ~Handle() noexcept;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Handle);
            };

            explicit Latch() noexcept;
            virtual ~Latch() noexcept;
            Latch * next;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Latch);
            static Latch *Query();
            static void   Store(Latch *latch) noexcept;
        };
    }

}

#endif


