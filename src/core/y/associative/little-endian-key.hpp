//! \file

#ifndef Y_Associative_LE_Key_Included
#define Y_Associative_LE_Key_Included 1

#include "y/memory/buffer/ro.hpp"
#include "y/type/ints.hpp"

namespace Yttrium
{

    class OutputStream;

    //__________________________________________________________________________
    //
    //
    //! Encode an integral type to a Little Endian R/O buffer
    //
    //__________________________________________________________________________
    class Little_Endian_Key : public Memory::ReadOnlyBuffer
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
        explicit Little_Endian_Key(const T &args) noexcept :
        Memory::ReadOnlyBuffer(),
        size(sizeof(T)),  code()
        {
            union {
                T                                     data;
                typename UnsignedInt<sizeof(T)>::Type uint;
            } alias = { args };
            setup(alias.uint);
        }


        Little_Endian_Key(const Little_Endian_Key &) noexcept; //!< copy
        virtual ~Little_Endian_Key()                 noexcept; //!< cleanup

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        virtual const void * ro_addr() const noexcept; //!< code
        virtual size_t       measure() const noexcept; //!< size

        friend std::ostream & operator<<(std::ostream &os, const Little_Endian_Key &key); //!< display
        friend OutputStream & operator<<(OutputStream &os, const Little_Endian_Key &key); //!< write

        //! recompose integer from code
        template <size_t N>
        typename UnsignedInt<N>::Type get() const noexcept;

    protected:
        const size_t size;      //!< stored bytes
        uint8_t      code[8];   //!< stored bytes

    private:
        Y_DISABLE_ASSIGN(Little_Endian_Key);
        void setup(const uint8_t  &) noexcept;
        void setup(const uint16_t &) noexcept;
        void setup(const uint32_t &) noexcept;
        void setup(const uint64_t &) noexcept;
    };

}

#endif

