
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

#define Y_DIGEST_CODE_CTOR(N) Object(), Reserved(), WadType(N), data( static_cast<uint8_t *>(workspace) ), item(data-1)

    class Reserved
    {
    public:
        explicit Reserved() noexcept : xlen(0) {}
        virtual ~Reserved() noexcept {}
        size_t   xlen;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Reserved);
    };

    class Digest:: Code : public Object, public Reserved, public Memory::Wad<uint8_t,Memory::Dyadic>
    {
    public:
        typedef Memory::Wad<uint8_t,Memory::Dyadic> WadType;

        inline explicit Code(const size_t n) :Y_DIGEST_CODE_CTOR(n)
        {
        }

        inline explicit Code(const Code &D) : Y_DIGEST_CODE_CTOR(D.maxBlocks)
        {
            memcpy(workspace,D.workspace,maxBlocks);
        }

        inline virtual ~Code() noexcept
        { memset(workspace,0,allocated); }


        //! get memory to hold value
        static inline size_t SizeFor(const char *hexa, size_t &rlen) noexcept
        {
            const size_t len  = Max<size_t>( rlen = StringLength(hexa), 1);
            const size_t alen = Y_ALIGN_ON(2,len);
            return alen >> 1;
        }

        inline explicit Code(const char *hexa) :
        Y_DIGEST_CODE_CTOR(SizeFor(hexa,xlen))
        {
            size_t ii = 0;
            bool   lo = true;
            while(xlen>0)
            {
                const char c = hexa[--xlen];
                const int  h = Hexadecimal::ToDecimal(c);
                if(h<0) throw Specific::Exception(Digest::CallSign,"invalid hexa char '%c'",c);
                if(lo)
                {
                    data[ii] = uint8_t(h);
                    lo = false;
                }
                else
                {
                    data[ii++] |= uint8_t(h) << 4;
                    lo = true;
                }
            }
        }


        uint8_t * const data;
        uint8_t * const item;

    private:
        Y_DISABLE_ASSIGN(Code);
    };

    Digest:: ~Digest() noexcept
    {
        assert(0!=code);
        delete code;
        code = 0;
    }

    Digest:: Digest(const size_t n) : code( new Code(n) )
    {
    }

    Digest:: Digest(const Digest &D) : code( new Code(*D.code) )
    {}


    Digest:: Digest(const char *hexa) : code( new Code(hexa) )
    {

    }



    const char * Digest:: callSign() const noexcept
    {
        return CallSign;
    }

    size_t Digest:: size() const noexcept
    {
        assert(0!=code);
        return code->maxBlocks;
    }

    size_t Digest:: measure() const noexcept
    {
        assert(0!=code);
        return code->maxBlocks;
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
        //for(size_t i=n;i>0;--i)
        {
            os << Hexadecimal::Text[D[i]];
        }
        return os;
    }





}
