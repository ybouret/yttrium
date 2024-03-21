
//! \file

#ifndef Y_IO_Output_Stream_Included
#define Y_IO_Output_Stream_Included 1


#include "y/stream/interface.hpp"
#include "y/memory/buffer/ro.hpp"
#include "y/check/printf.hpp"
#include "y/type/ints.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! Base class for output strean
    //
    //
    //__________________________________________________________________________
    class OutputStream : public virtual Stream
    {
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
    protected:
        explicit OutputStream() noexcept; //!< setup

    public:
        virtual ~OutputStream() noexcept; //!< cleanup

        //______________________________________________________________________
        //
        //
        // Interface
        //
        //______________________________________________________________________
        virtual void write(const char) = 0; //!< write one char
        virtual void flush()           = 0; //!< flush cache is any

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        //! write blocks of memory
        void write(const void *blockAddr, const size_t blockSize);

        OutputStream & operator<<(const char );                      //!< helper
        OutputStream & operator<<(const char *);                     //!< helper
        OutputStream & operator<<(const Memory::ReadOnlyBuffer &);   //!< helper
        OutputStream & operator()(const char *fmt,...) Y_PRINTF_API; //!< helper
        
        void issue(const uint8_t  &); //!< fixed size BE issue
        void issue(const uint16_t &); //!< fixed size BE issue
        void issue(const uint32_t &); //!< fixed size BE issue
        void issue(const uint64_t &); //!< fixed size BE issue


        //! emit Constant Byte Rate integer
        template <typename T> inline
        size_t emitCBR(const T &x) {
            union
            {
                T                                     user;
                typename UnsignedInt<sizeof(T)>::Type word;
            } alias = { x };
            issue(alias.word);
            return sizeof(T);
        }

        //! emit Variable Byte Rate integer
        template <typename T> inline
        size_t emitVBR(const T &x) {
            union
            {
                T                                     user;
                typename UnsignedInt<sizeof(T)>::Type word;
            } alias = { x };
            return emit64( alias.word );
        }



    private:
        Y_DISABLE_COPY_AND_ASSIGN(OutputStream);
        size_t emit64(const uint64_t);

    };
}

#endif

