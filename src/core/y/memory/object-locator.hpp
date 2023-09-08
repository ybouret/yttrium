
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
            typedef const void * (ObjectLocator:: *Query)(size_t &) const;


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
            const void * prevAddr(size_t &prevSize) const noexcept;
            const void * nextAddr(size_t &nextSize) const noexcept;

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

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ObjectLocator);
            
        };

        class ObjectGuard
        {
        public:
            explicit ObjectGuard(const ObjectLocator &, ObjectLocator::Query query) noexcept;
            virtual ~ObjectGuard() noexcept;

            const size_t blockSize;
            const void  *blockAddr;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ObjectGuard);
        };


        class ObjectSentry : public ObjectGuard, public Sentry
        {
        public:
            explicit ObjectSentry(const ObjectLocator &, ObjectLocator::Query query) noexcept;
            virtual ~ObjectSentry() noexcept;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ObjectSentry);
        };

        class ObjectSentries : public ObjectLocator
        {
        public:
            explicit ObjectSentries(const void *blockAddr);
            virtual ~ObjectSentries() noexcept;

            const ObjectSentry prev;
            const ObjectSentry next;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ObjectSentries);
        };






    }

}

#endif
