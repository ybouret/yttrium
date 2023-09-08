
//! \file

#ifndef Y_Memory_Object_Locator_Included
#define Y_Memory_Object_Locator_Included 1

#include "y/memory/sentry.hpp"
#include "y/ostream-proto.hpp"

namespace Yttrium
{
    namespace Memory
    {
        class Arena;
        class Chunk;
        class Strap;

        //______________________________________________________________________
        //
        //
        //! find memory information of a block of memory allocated by object
        //
        //______________________________________________________________________
        class ObjectLocator
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef const void * (ObjectLocator:: *Query)(size_t &) const; //!< alias to query memory to check


            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit ObjectLocator(const void *blockAddr); //!< setup
            virtual ~ObjectLocator() noexcept;             //!< cleanup
            Y_OSTREAM_PROTO(ObjectLocator);                //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const void * prevAddr(size_t &prevSize) const noexcept; //!< query known prev memory area
            const void * nextAddr(size_t &nextSize) const noexcept; //!< query known next memory area


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const void  * const where; //!< block address
            const Arena * const arena; //!< if small object
            const Chunk * const chunk; //!< if small object
            const Strap * const strap; //!< if large object
            const size_t        width; //!< used bytes

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ObjectLocator);
            
        };

        //______________________________________________________________________
        //
        //
        //! guess memory Area from locator and given Query method
        //
        //______________________________________________________________________
        class ObjectGuard
        {
        public:
            explicit ObjectGuard(const ObjectLocator &, ObjectLocator::Query) noexcept; //!< setup
            virtual ~ObjectGuard()                                            noexcept; //!< cleanup

            const size_t       blockSize; //!< stored block size
            const void * const blockAddr; //!< stored block address

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ObjectGuard);
        };


        //______________________________________________________________________
        //
        //
        //! Sentry based on a computed Object Guard
        //
        //______________________________________________________________________
        class ObjectSentry : public ObjectGuard, public Sentry
        {
        public:
            explicit ObjectSentry(const ObjectLocator &, ObjectLocator::Query) noexcept; //!< setup
            virtual ~ObjectSentry()                                            noexcept; //!< cleanup
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ObjectSentry);
        };

        //______________________________________________________________________
        //
        //
        //! Sentries based on both computed ObjectSentry
        //
        //______________________________________________________________________
        class ObjectSentries : public ObjectLocator
        {
        public:
            explicit ObjectSentries(const void *obj); //!< setup
            virtual ~ObjectSentries()       noexcept; //!< cleanup

            const ObjectSentry prev; //!< sentry at previous memory area
            const ObjectSentry next; //!< sentry at next     memory area

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ObjectSentries);
        };






    }

}

#endif
