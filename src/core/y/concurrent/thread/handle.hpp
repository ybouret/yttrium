//! \file

#ifndef Y_Thread_Handle_Included
#define Y_Thread_Handle_Included 1

#include "y/text/base64/encode.hpp"
#include "y/check/static.hpp"
#include <iosfwd>

namespace Yttrium
{
    namespace Concurrent
    {

        class ThreadHandle
        {
        public:
#if defined(Y_BSD)
            typedef void *Type;
#endif

#if defined(Y_WIN)
            typedef uint32_t Type;
#endif
            static const size_t BufferSize = Base64::Encode::OutputLengthFor<sizeof(Type)>::Value;

            ThreadHandle(const ThreadHandle &)             noexcept;
            ThreadHandle & operator=(const ThreadHandle &) noexcept;
            ~ThreadHandle()                                noexcept;
            
            template <typename T>
            inline ThreadHandle(const T &args) noexcept :
            buffer()
            {
                Y_STATIC_CHECK(sizeof(T)==sizeof(Type),ThreadHandleSize)
                clear();
                union  {
                    T    user;
                    Type mine;
                } alias = { args };
                write(&alias.mine);
            }

            const char * c_str() const noexcept;
            friend std::ostream & operator<<(std::ostream &, const ThreadHandle &);
            friend bool           operator==(const ThreadHandle &, const ThreadHandle &) noexcept;
            friend bool           operator!=(const ThreadHandle &, const ThreadHandle &) noexcept;

        private:
            char buffer[BufferSize];
            void clear() noexcept;
            void write(const void *) noexcept;
        };

    }

}

#endif
