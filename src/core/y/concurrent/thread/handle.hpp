//! \file

#ifndef Y_Thread_Handle_Included
#define Y_Thread_Handle_Included 1

#include "y/text/base64/encode.hpp"
#include "y/check/static.hpp"
#include "y/memory/buffer/ro.hpp"
#include <iosfwd>

namespace Yttrium
{
    namespace Concurrent
    {

        //______________________________________________________________________
        //
        //
        //
        //! Human Readable, System dependant Thread Handle
        //
        //
        //______________________________________________________________________
        class ThreadHandle : public Memory::ReadOnlyBuffer
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            
            //! maximum type size
            typedef void *Type;

            //! no-padding base64 buffer size
            static const size_t BufferSize = Base64::Encode::OutputLengthFor<sizeof(Type)>::Value;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            ThreadHandle(const ThreadHandle &)             noexcept; //!< copy
            ThreadHandle & operator=(const ThreadHandle &) noexcept; //!< assign
            virtual ~ThreadHandle()                        noexcept; //!< cleanup
            explicit ThreadHandle()                        noexcept; //!< setup empty

            //! setup by shadowing
            template <typename T>
            inline ThreadHandle(const T &args) noexcept :
            buflen(0),
            buffer()
            {
                Y_STATIC_CHECK(sizeof(T)<=sizeof(Type),ThreadHandleSize)
                write(&args,sizeof(T));
            }

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            friend std::ostream & operator<<(std::ostream &, const ThreadHandle &);                //!< display
            friend bool           operator==(const ThreadHandle &, const ThreadHandle &) noexcept; //!< equality
            friend bool           operator!=(const ThreadHandle &, const ThreadHandle &) noexcept; //!< difference
            const char *          c_str()                                          const noexcept; //!< legacy access
            virtual size_t        measure() const noexcept;

        private:
            size_t buflen;
            char   buffer[BufferSize];
            void clear() noexcept;
            void write(const void *, const size_t) noexcept;
            virtual const void * ro_addr() const noexcept;
        };

    }

}

#endif
