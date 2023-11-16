
//! \file

#ifndef Yttrium_Concurrent_Mutex_Included
#define Yttrium_Concurrent_Mutex_Included 1


#include "y/lockable.hpp"
#include "y/concurrent/primitive.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        namespace Nucleus
        {
            class Mutex;
        }

        //______________________________________________________________________
        //
        //
        //
        //! Mutex Prototype
        //
        //
        //______________________________________________________________________
        class MutexProto : public Primitive, public Lockable
        {
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
        protected:
            //! set lockable and fetch Quark instance
            explicit MutexProto(const char *id);

        public:
            virtual ~MutexProto() noexcept;

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual Nucleus::Mutex & operator*() noexcept = 0; //!< access internal mutex

            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MutexProto);
        };


        //______________________________________________________________________
        //
        //
        //
        //! Mutex interface for internal Quark
        //
        //
        //______________________________________________________________________
        class NucleusMutex : public MutexProto
        {
        public:
            explicit NucleusMutex(const char *id=0);        //!< setup
            virtual ~NucleusMutex() noexcept;               //!< cleanup
            virtual Nucleus::Mutex & operator*() noexcept;  //!< Quark.giant

        private:
            Y_DISABLE_COPY_AND_ASSIGN(NucleusMutex);
            Y_LOCKABLE_API();
        };


        //______________________________________________________________________
        //
        //
        //
        //! [Regular]Mutex object
        //
        //
        //______________________________________________________________________
        class Mutex : public MutexProto
        {
        public:
            explicit Mutex(const char *id=0);              //!< setup with optional id
            virtual ~Mutex() noexcept;                     //!< cleanup
            virtual Nucleus::Mutex & operator*() noexcept; //!< self mutex

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Mutex);
            Y_LOCKABLE_API();
            Nucleus::Mutex *mutex;
        };




        
    }
}

#endif
