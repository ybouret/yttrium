//! \file

#ifndef Y_Memory_Digest_Included
#define Y_Memory_Digest_Included 1

#include "y/container/writable.hpp"
#include "y/memory/buffer/rw.hpp"
#include "y/ostream-proto.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! Digest, mostly for Hashing Ops
    //
    //
    //__________________________________________________________________________
    class Digest :
    public Writable<uint8_t>,
    public Memory::ReadWriteBuffer
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        static const char * const CallSign; //!< "Digest"

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        explicit Digest(const size_t); //!< setup with exact size
        virtual ~Digest() noexcept;    //!< cleanup
        Digest(const Digest &);        //!< copy
        Digest(const char  *hexa);     //!< parse from hexadecimal string
        Y_OSTREAM_PROTO(Digest);       //!< display

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        virtual const char *callSign() const throw(); //!< CallSign
        virtual size_t      size()     const throw(); //!< exact size
        virtual const void *ro_addr()  const throw(); //!< for buffer
        virtual size_t      measure()  const throw(); //!< for buffer

        uint8_t &       operator[](const size_t)       noexcept; //!< access
        const uint8_t & operator[](const size_t) const noexcept; //!< access, const
        uint8_t *       operator()(void)               noexcept; //!< access
        const uint8_t * operator()(void)         const noexcept; //!< access, const

        void ldz() noexcept; //!< set to zero

    private:
        Y_DISABLE_ASSIGN(Digest);
        class Code;
        Code *code;
    };
}
#endif

