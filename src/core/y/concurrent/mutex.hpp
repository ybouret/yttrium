
//! \file

#ifndef Yttrium_Concurrent_Fake_Lock_Included
#define Yttrium_Concurrent_Fake_Lock_Included 1


#include "y/lockable.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        namespace Nucleus
        {
            class Mutex;
            class Quark;
        }


        class MutexProto : public Lockable
        {
        protected:
            explicit MutexProto(const char *id=0);

        public:
            virtual ~MutexProto() noexcept;
            virtual Nucleus::Mutex & operator*() noexcept = 0;

        protected:
            Nucleus::Quark &quark;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MutexProto);
        };

        class NucleusMutex : public MutexProto
        {
        public:
            explicit NucleusMutex(const char *id=0);
            virtual ~NucleusMutex() noexcept;
            virtual Nucleus::Mutex & operator*() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(NucleusMutex);
            Y_LOCKABLE_API();
        };


        //______________________________________________________________________
        //
        //
        //
        //! Mutex object
        //
        //
        //______________________________________________________________________
        class Mutex : public MutexProto
        {
        public:
            explicit Mutex(const char *id=0); //!< setup with optional id
            virtual ~Mutex() noexcept;        //!< cleanup
            virtual Nucleus::Mutex & operator*() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Mutex);
            Y_LOCKABLE_API();
            Nucleus::Mutex *mutex;
        };




        
    }
}

#endif
