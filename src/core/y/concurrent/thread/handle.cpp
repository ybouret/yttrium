#include "y/concurrent/thread/handle.hpp"
#include "y/container/algo/reverse.hpp"
#include "y/type/utils.hpp"
#include "y/text/ops.hpp"

#include <cstring>
#include <iostream>

namespace Yttrium
{
    namespace Concurrent
    {

        ThreadHandle::ThreadHandle() noexcept :
        Memory::ReadOnlyBuffer(),
        buflen(0),
        buffer()
        {
            clear();
        }

        ThreadHandle:: ThreadHandle(const CopyOf_ &, const char * const data) noexcept:
        Memory::ReadOnlyBuffer(),
        buflen( StringLength(data) ),
        buffer()
        {
            clear();
            assert(buflen<BufferSize);
            memcpy(buffer,data,buflen);
        }

        ThreadHandle:: ThreadHandle(const ThreadHandle &other) noexcept :
        Memory::ReadOnlyBuffer(),
        buflen(other.buflen),
        buffer()
        {
            memcpy(buffer,other.buffer,BufferSize);
        }


        void ThreadHandle:: swapWith(ThreadHandle &other) noexcept
        {
            Swap(buflen,other.buflen);
            Memory::OutOfReach::Swap(buffer,other.buffer,BufferSize);
        }

        ThreadHandle & ThreadHandle:: operator=(const ThreadHandle &other) noexcept
        {
            {
                ThreadHandle tmp(other);
                swapWith(tmp);
            }
            assert(buflen == other.buflen);
            assert(*this  == other);
            return *this;
        }

        const char * ThreadHandle:: c_str() const noexcept
        {
            return buffer;
        }

        std::ostream & operator<<(std::ostream &os, const ThreadHandle &th)
        {
            return os << th.buffer;
        }

        void ThreadHandle:: clear() noexcept
        {
            memset(buffer,0,BufferSize);
        }

        ThreadHandle:: ~ThreadHandle() noexcept
        {
            clear();
        }

        void ThreadHandle:: write(const void *data, const size_t size) noexcept
        {
            assert(size<=sizeof(Type));
            assert(BufferSize>Base64::Encode::LengthFor(size,false));
            clear();
            buflen = Base64::Encode::To(buffer, data, size, false);
            assert(strlen(buffer)==buflen);
            Algo::Reverse(buffer,buflen);    // TODO: depends on endianess ?
            assert(strlen(buffer)==buflen);
        }

        const void * ThreadHandle:: ro_addr() const noexcept
        {
            return buffer;
        }

        size_t ThreadHandle::measure() const noexcept
        {
            return buflen;
        }

        bool operator==(const ThreadHandle &lhs, const ThreadHandle &rhs) noexcept
        {
            return 0 == memcmp(lhs.buffer, rhs.buffer, ThreadHandle::BufferSize);
        }

        bool operator!=(const ThreadHandle &lhs, const ThreadHandle &rhs) noexcept
        {
            return 0 != memcmp(lhs.buffer, rhs.buffer, ThreadHandle::BufferSize);
        }

        void ThreadHandle:: skip(const size_t prefix) noexcept
        {
            assert(prefix<=buflen);

            {
                const size_t       newlen = buflen-prefix;
                char *       const target = buffer;
                const char * const source = buffer + prefix;
                memmove(target,source,newlen);
                buflen = newlen;
            }
            memset(buffer+buflen,0,BufferSize-buflen);
            assert(strlen(buffer)==buflen);
        }

    }

}


