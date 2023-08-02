
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

#define Y_DIGEST_CTOR(N) WadType(N), data( static_cast<uint8_t *>(workspace) ), item(data-1)

    class Digest:: Code : public Object, public Memory::Wad<uint8_t,Memory::Dyadic>
    {
    public:
        typedef Memory::Wad<uint8_t,Memory::Dyadic> WadType;

        inline explicit Code(const size_t n) :Y_DIGEST_CTOR(n)
        {
        }

        inline explicit Code(const Code &D) : Y_DIGEST_CTOR(D.maxBlocks)
        {
            memcpy(workspace,D.workspace,maxBlocks);
        }

        inline virtual ~Code() noexcept
        { memset(workspace,0,allocated); }

        static inline size_t hexaLen(const char *hexa) noexcept
        {
            const size_t len  = Max<size_t>( StringLength(hexa), 1);
            const size_t alen = Y_ALIGN_ON(2,len);
            return alen >> 1;
        }

        inline explicit Code(const char *hexa) :
        Y_DIGEST_CTOR(hexaLen(hexa))
        {
            if(0!=hexa)
            {
                
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
        for(size_t i=D.size();i>0;--i)
        {
            os << Hexadecimal::Text[D[i]];
        }
        return os;
    }





}
