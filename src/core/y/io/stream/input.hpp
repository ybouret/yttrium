

//! \file

#ifndef Y_Input_Stream_Included
#define Y_Input_Stream_Included 1


#include "y/io/interface.hpp"
#include "y/type/ints.hpp"
#include "y/io/chars.hpp"

namespace Yttrium
{

    namespace Core { template <typename> class String; }

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

        size_t query(void *, const size_t); //!< query block of memory
        size_t fetch(uint8_t  &);           //!< try to read 1 byte
        size_t fetch(uint16_t &);           //!< try to read 2 bytes
        size_t fetch(uint32_t &);           //!< try to read 4 bytes
        size_t fetch(uint64_t &);           //!< try to read 8 bytes


        //______________________________________________________________________
        //
        //! read any Constant Byte Rate integer
        //______________________________________________________________________
        template <typename T> inline
        T readCBR(const char *ctx=0)
        {
            union
            {
                typename  UnsignedInt< sizeof(T) >::Type word;
                T                                        user;
            } alias = { 0 };
            const size_t nr = fetch(alias.word);
            if(nr<sizeof(T)) missingBytes(sizeof(T)-nr,ctx);
            return alias.user;
        }

        //______________________________________________________________________
        //
        //! read any Variable Byte Rate integer
        //______________________________________________________________________
        template <typename T> inline
        T readVBR(const char *ctx=0)
        {
            static const uint64_t m64 = IntegerFor<T>::Maximum;
            const uint64_t        u64  = read64(ctx);
            if(u64>m64) overflowing(ctx);
            return T(u64);
        }

        //______________________________________________________________________
        //
        //! get a line delimited by cr,lf, or crlf
        //______________________________________________________________________
        bool         gets(IO::Chars &line);

        //______________________________________________________________________
        //
        //! get a line as String of chars
        //______________________________________________________________________
        bool         gets(Core::String<char> &line);

        //______________________________________________________________________
        //
        //! load content
        //______________________________________________________________________
        template <typename SEQUENCE> inline
        void loadInto(SEQUENCE &seq)
        {
            char c=0; while(query(c)) seq << c;
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(InputStream);
        void missingBytes(const size_t, const char *) const;
        void overflowing(const char *) const;
        uint64_t read64(const char *ctx);
    };
}

#endif

