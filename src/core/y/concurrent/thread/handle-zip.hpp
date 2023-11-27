
//! \file

#ifndef Y_Thread_Handle_Zip_Included
#define Y_Thread_Handle_Zip_Included 1

#include "y/concurrent/thread/handle.hpp"
#include "y/data/small/light/list/solo.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        //______________________________________________________________________
        //
        //! alias to store references to thread handles
        //______________________________________________________________________
        typedef Small::SoloLightList<const ThreadHandle> ThreadHandleList;

        //______________________________________________________________________
        //
        //
        //
        //! Reducing thread handle complexity by removing homology
        //
        //
        //______________________________________________________________________
        class ThreadHandleZip : public ThreadHandleList
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit ThreadHandleZip() noexcept;    //!< setup empty
            explicit ThreadHandleZip(const size_t); //!< setup with capacity
            virtual ~ThreadHandleZip() noexcept;    //!< cleanup


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            size_t homology()                    const noexcept; //!< current homology
            void   loadPrimary();                                //!< append primary thread handle
            void   unloadPrimary()                     noexcept; //!< ensure no primary in list
            bool   compress(const size_t prefix)       noexcept; //!< enforce skip on all handles, false if produces same handles!
            bool   compress()                          noexcept; //!< compress( homology() )

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const ThreadHandle primary; //!< primary thread handle if necessary

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ThreadHandleZip);
        };

    }

}

#endif

