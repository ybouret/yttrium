

//! \file

#ifndef Y_IO_Input_Stream_Included
#define Y_IO_Input_Stream_Included 1


#include "y/io/stream.hpp"
#include "y/type/ints.hpp"

namespace Yttrium
{
    //______________________________________________________________________
    //
    //
    //
    //! Base class for input stream
    //
    //
    //______________________________________________________________________
    class InputStream : public virtual Stream
    {
    protected:
        explicit InputStream() noexcept; //!< setup

    public:
        virtual ~InputStream() noexcept; //!< cleanup

        //______________________________________________________________________
        //
        //
        // Interface
        //
        //______________________________________________________________________
        virtual bool query(char & data) = 0; //!< query a single char, false = EOS
        virtual void store(const  char) = 0; //!< unread a char
        virtual bool ready()            = 0; //!< next char ?

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        static const char *From(const char *ctx) noexcept; //!< ctx/Unknown

        size_t query(void *blockAddr, const size_t blockSize);

        size_t fetch(uint8_t  &); //!< try to read 1 byte
        size_t fetch(uint16_t &); //!< try to read 2 bytes
        size_t fetch(uint32_t &); //!< try to read 4 bytes
        size_t fetch(uint64_t &); //!< try to read 8 bytes

        template <typename T> inline
        T readCBR(const char *ctx=0)
        {
            union
            {
                typename  UnsignedInt< sizeof(T) >::Type word;
                T                                        user;
            } alias = { 0 };
            const size_t nr = fetch(alias.word);
            if(nr<sizeof(T)) MissingBytes(sizeof(T)-nr,ctx);
            return alias.user;
        }

        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(InputStream);
        void MissingBytes(const size_t, const char *) const;
    };
}

#endif

