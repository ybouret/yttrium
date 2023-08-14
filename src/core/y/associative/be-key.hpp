//! \file

#ifndef Y_Associative_BE_Key_Included
#define Y_Associative_BE_Key_Included 1

#include "y/memory/buffer/ro.hpp"
#include "y/type/ints.hpp"

namespace Yttrium
{

    class OutputStream;

    //__________________________________________________________________________
    //
    //
    //! Encode an integral type to a Big Endian R/O buffer
    //
    //__________________________________________________________________________
    class BE_Key : public Memory::ReadOnlyBuffer
    {
    public:
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //! setup from integral type
        template <typename T> inline
        explicit BE_Key(const T &args) noexcept :
        size(sizeof(T)), code()
        {
            union {
                T                                     data;
                typename UnsignedInt<sizeof(T)>::Type uint;
            } alias = { args };
            setup(alias.uint);
        }


        BE_Key(const BE_Key &) noexcept; //!< copy
        virtual ~BE_Key()      noexcept; //!< cleanup

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        virtual const void * ro_addr() const noexcept; //!< code
        virtual size_t       measure() const noexcept; //!< size


        friend std::ostream & operator<<(std::ostream &os, const BE_Key &key); //!< display
        friend OutputStream & operator<<(OutputStream &os, const BE_Key &key); //!< write

    protected:
        const size_t size;      //!< stored bytes
        uint8_t      code[8];   //!< stored bytes

    private:
        Y_DISABLE_ASSIGN(BE_Key);
        void setup(const uint8_t) noexcept;
        void setup(const uint16_t) noexcept;
        void setup(const uint32_t) noexcept;
        void setup(const uint64_t) noexcept;
    };

}

#endif

