//! \file

#ifndef Y_Ordered_Core_Raw_Buffer_Included
#define Y_Ordered_Core_Raw_Buffer_Included 1

#include "y/ordered/priority-queue.hpp"
#include "y/container/interface.hpp"
#include <iostream>

namespace Yttrium
{

    namespace Core
    {

        template <typename T>
        class RawBuffer : public Container
        {
        public:
            Y_ARGS_DECL(T,Type);
            inline explicit RawBuffer() noexcept : entry(0), count(0), tally(0) {}
            inline virtual ~RawBuffer() noexcept {}

            inline void           free()           noexcept { PriorityQueue<MutableType>::Finish(entry,count); }
            inline void           make(size_t n)   noexcept { free(); prepareForMaxi(n); }
            inline ConstType &    peek()     const noexcept { assert(count>0); assert(0!=entry); return entry[0]; }
            inline virtual size_t size()     const noexcept { return count; }
            inline virtual size_t capacity() const noexcept { return tally; }

            inline virtual const char * callSign() const noexcept { return "todo"; }


            template <typename COMPARE>
            void insertWith(COMPARE &proc, const T &args)
            {
                mustAcceptNext();
                PriorityQueue<T>::Insert(entry,count,args,proc);
            }

            template <typename COMPARE> void removeWith(COMPARE &proc) noexcept
            {
                assert(count>0); assert(0!=entry);
                PriorityQueue<T>::Remove(entry,count,proc);
            }

            template <typename COMPARE>
            Type uprootWith(COMPARE &proc)
            {
                assert(count>0); assert(0!=entry);
                ConstType res = entry[0];
                removeWith(proc);
                return res;
            }

            inline friend std::ostream & operator<<(std::ostream &os, const RawBuffer &self)
            {
                return Core::Display(os,self.entry,self.count);
            }

        protected:
            MutableType * const entry;
            size_t              count;
            const size_t        tally;

            virtual void mustAcceptNext() = 0;
            virtual void prepareForMaxi(const size_t) = 0;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(RawBuffer);
        };
    }

}

#endif
