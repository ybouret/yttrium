
#include "y/memory/digest.hpp"
#include "y/text/hexadecimal.hpp"

#include "y/memory/wad.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/object.hpp"
#include "y/type/utils.hpp"
#include "y/text/ops.hpp"
#include "y/calculus/align.hpp"
#include "y/system/exception.hpp"

#include <cstring>

namespace  Yttrium
{
    const char * const Digest:: CallSign = "Digest";



    namespace
    {
        //! used to fetch hexa length
        class HexaLen
        {
        public:
            explicit HexaLen() noexcept :  xlen(0) {}
            virtual ~HexaLen() noexcept {}

            size_t       xlen; //!< hexa len if needed

        private:
            Y_DISABLE_COPY_AND_ASSIGN(HexaLen);
        };

        //! used to store exact size
        class Metrics
        {
        public:
            explicit Metrics(const size_t n) noexcept : size(n) {}
            virtual ~Metrics()               noexcept {}

            const size_t size; //!< requested size

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Metrics);
        };

        typedef Memory::Wad<uint8_t,Memory::Dyadic> WadType;
    }

#define Y_DIGEST_CODE_CTOR(N) Object(), HexaLen(), Metrics(N), WadType(size), data( static_cast<uint8_t *>(workspace) ), item(data-1)

    class Digest:: Code :
    public Object,
    public HexaLen,
    public Metrics,
    public WadType
    {
    public:
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //! setup with size=n
        inline explicit Code(const size_t n) : Y_DIGEST_CODE_CTOR(n)
        {
        }

        //! copy
        inline explicit Code(const Code &D) : Y_DIGEST_CODE_CTOR(D.size)
        {
            memcpy(workspace,D.workspace,size);
        }

        //! cleanup
        inline virtual ~Code() noexcept
        { memset(workspace,0,allocated); }


        //! setup from hexa string
        inline explicit Code(const char *hexa) :
        Y_DIGEST_CODE_CTOR(SizeFor(hexa,xlen))
        {
            bool   hi = true;
            size_t ii = 0;
            if(0x1&xlen) push('0',hi,ii);
            for(size_t i=0;i<xlen;++i) push(hexa[i],hi,ii);
        }

        //______________________________________________________________________
        //
        //
        // members
        //
        //______________________________________________________________________

        uint8_t * const data;
        uint8_t * const item;

    private:
        Y_DISABLE_ASSIGN(Code);
        //! get memory to hold value
        static inline size_t SizeFor(const char *hexa, size_t &rlen) noexcept
        {
            const size_t len  = Max<size_t>( rlen = StringLength(hexa), 1);
            const size_t alen = Y_ALIGN_ON(2,len);
            return alen >> 1;
        }

        //! feed data
        void push(const char cc, bool &hi, size_t &ii)
        {
            const int h = Hexadecimal::ToDecimal(cc);
            if(h<0) throw Specific::Exception(Digest::CallSign,"invalid hexa '%c'",h);
            if(hi)
            {
                data[ii] = uint8_t(h) << 4;
                hi = false;
            }
            else
            {
                data[ii++] |= uint8_t(h);
                hi = true;
            }
        }
    };

    Digest:: ~Digest() noexcept
    {
        assert(0!=code);
        delete code;
        code = 0;
    }

#define Y_DIGEST_CTOR(CODE) Identifiable(), Collection(), Writable<uint8_t>(), Memory::ReadWriteBuffer(), code( CODE )

    Digest:: Digest(const size_t n) : Y_DIGEST_CTOR( new Code(n) )
    {
    }

    Digest:: Digest(const Digest &D) : Y_DIGEST_CTOR( new Code(*D.code) )
    {}


    Digest:: Digest(const char *hexa) : Y_DIGEST_CTOR( new Code(hexa) )
    {
    }



    const char * Digest:: callSign() const noexcept
    {
        return CallSign;
    }

    size_t Digest:: size() const noexcept
    {
        assert(0!=code);
        return code->size;
    }

    size_t Digest:: measure() const noexcept
    {
        assert(0!=code);
        return code->size;
    }

    const void * Digest :: ro_addr() const noexcept
    {
        assert(0!=code);
        return code->data;
    }

    uint8_t & Digest:: operator[](const size_t i) noexcept
    {
        assert(0!=code);
        assert(i>0);
        assert(i<=size());
        return code->item[i];
    }

    const uint8_t & Digest:: operator[](const size_t i) const noexcept
    {
        assert(0!=code);
        assert(i>0);
        assert(i<=size());
        return code->item[i];
    }

    std::ostream & operator<<(std::ostream &os, const Digest &D)
    {
        //std::cerr << "Writing Digest:";
        const size_t n = D.size();
        for(size_t i=1;i<=n;++i)
        {
            os << Hexadecimal::Text[D[i]];
        }
        return os;
    }





}
