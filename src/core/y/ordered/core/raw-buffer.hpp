//! \file

#ifndef Y_Ordered_Core_Raw_Buffer_Included
#define Y_Ordered_Core_Raw_Buffer_Included 1

#include "y/ordered/priority-queue.hpp"
#include "y/container/interface.hpp"
#include "y/container/recyclable.hpp"
#include "y/container/readable.hpp"
#include <iostream>

namespace Yttrium
{

    namespace Core
    {

        //______________________________________________________________________
        //
        //
        //
        //! Raw Buffer interface
        //
        //
        //______________________________________________________________________
        template <typename T>
        class RawBuffer : public Readable<T>, public Container, public Recyclable
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            Y_ARGS_DECL(T,Type); //!< aliases

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
        protected:
            //! setup, empty
            inline explicit RawBuffer() noexcept : entry(0), count(0), tally(0) {}

        public:
            //! cleanup
            inline virtual ~RawBuffer() noexcept {}

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! [Recyclable] clean content memory
            inline virtual void free() noexcept
            { 
                PriorityQueue<MutableType>::Finish(entry,count);
            }

            //! [Container] current  objects
            inline virtual size_t size()     const noexcept { return count; }

            //! [Container] maximum objects
            inline virtual size_t capacity() const noexcept { return tally; }

            //! empty and prepare for n objects
            inline void           make(size_t n)   noexcept { free(); prepareForMaxi(n); }

            //! peek to opject
            inline ConstType &    peek()     const noexcept { assert(count>0); assert(0!=entry); return entry[0]; }


            //! forward to PrioQ
            template <typename COMPARE> inline
            void insertWith(COMPARE &proc, const T &args)
            {
                mustAcceptNext();
                PriorityQueue<T>::Insert(entry,count,args,proc);
            }

            //! forward to PrioQ
            template <typename COMPARE> inline
            void removeWith(COMPARE &proc) noexcept
            {
                assert(count>0); assert(0!=entry);
                PriorityQueue<T>::Remove(entry,count,proc);
            }

            //! forward to PrioQ
            template <typename COMPARE> inline
            Type uprootWith(COMPARE &proc)
            {
                assert(count>0); assert(0!=entry);
                ConstType res = entry[0];
                removeWith(proc);
                return res;
            }

            //! dire insertion
            inline void insertAtTail(const T &args)
            {
                mustAcceptNext();
                new (entry+count) MutableType(args);
                ++count;
            }

            //! dire removal
            inline void removeAtTail() noexcept
            { PriorityQueue<T>::RemoveLast(entry,count); }

            //! dire uproot
            inline ConstType uprootAtTail()
            {
                assert(count>0); assert(0!=entry);
                MutableType *src = &entry[--count];
                ConstType    res = *src;
                (void)Memory::OutOfReach::Naught(src);
                return res;
            }


            
            //! [Readable] return in [1:count]
            ConstType & operator[](const size_t index) const noexcept
            {
                assert(index>0);
                assert(index<=count);
                return entry[index-1];
            }


        protected:
            MutableType * const entry; //!< first object address
            size_t              count; //!< number of object
            const size_t        tally; //!< maximum number of objects

            virtual void mustAcceptNext()             = 0; //!< check memory for mandatory space
            virtual void prepareForMaxi(const size_t) = 0; //!< increase capacity

        private:
            Y_DISABLE_COPY_AND_ASSIGN(RawBuffer);
        };
    }

}

#endif
