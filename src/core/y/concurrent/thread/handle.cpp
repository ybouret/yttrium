#include "y/concurrent/thread/handle.hpp"
#include <cstring>
#include <iostream>

namespace Yttrium
{
    namespace Concurrent
    {
        ThreadHandle:: ThreadHandle(const ThreadHandle &other) noexcept :
        buffer()
        {
            memcpy(buffer,other.buffer,BufferSize);
        }

        ThreadHandle & ThreadHandle:: operator=(const ThreadHandle &other) noexcept
        {
            memmove(buffer, other.buffer, BufferSize);
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
            Base64::Encode::To(buffer, data, size, false);
        }

        bool operator==(const ThreadHandle &lhs, const ThreadHandle &rhs) noexcept
        {
            return 0 == memcmp(lhs.buffer, rhs.buffer, ThreadHandle::BufferSize);
        }

        bool operator!=(const ThreadHandle &lhs, const ThreadHandle &rhs) noexcept
        {
            return 0 != memcmp(lhs.buffer, rhs.buffer, ThreadHandle::BufferSize);
        }


    }

}


