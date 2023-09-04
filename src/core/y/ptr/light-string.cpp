
#include "y/ptr/light-string.hpp"
#include "y/object.hpp"
#include "y/counted.hpp"
#include "y/text/ops.hpp"
#include "y/memory/wad.hpp"
#include "y/memory/allocator/pooled.hpp"

#include <cstring>
#include <iostream>

namespace Yttrium
{

    namespace
    {
        typedef Memory::Wad<char,Memory::Pooled> WadType;

        class Metrics
        {
        public:
            inline Metrics(const size_t l, const size_t r) noexcept :
            lsize(l),
            rsize(r),
            size(lsize+rsize)
            {
            }

            inline virtual ~Metrics() noexcept
            {
            }

            const size_t lsize;
            const size_t rsize;
            const size_t size;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Metrics);
        };

    }

    class LightString:: Code : public Object, public Counted, public Metrics, public WadType
    {
    public:
        explicit Code(const char *lhs) :
        Object(),
        Counted(),
        Metrics( StringLength(lhs), 0),
        WadType( size+1 ),
        data( static_cast<char *>(workspace) )
        {
            memcpy(workspace,lhs,lsize);
        }

        explicit Code(const char *lhs, const char *rhs) :
        Object(),
        Counted(),
        Metrics( StringLength(lhs), StringLength(rhs) ),
        WadType( size+1 ),
        data( static_cast<char *>(workspace) )
        {
            memcpy(workspace,lhs,lsize);
            memcpy(static_cast<char*>(workspace)+lsize,rhs,rsize);
        }

        virtual ~Code() noexcept
        {
            memset(workspace,0,numBytes);
        }

        const char * const data;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Code);
    };


    const char * const LightString:: CallSign = "CStrPtr";

    LightString:: LightString(const char *lhs) : Identifiable(), Collection(), Readable<char>(), code( new Code(lhs) )
    {
        code->withhold();
    }

    LightString:: LightString(const char *lhs, const char *rhs) : Identifiable(), Collection(), Readable<char>(), code( new Code(lhs,rhs) )
    {
        code->withhold();
    }


    LightString:: ~LightString() noexcept
    {
        assert(0!=code);
        if(code->liberate()) delete code;
        code = 0;
    }

    const char * LightString:: operator()(void) const noexcept
    {
        assert(0!=code);
        return code->data;
    }


    LightString:: LightString(const LightString &other) noexcept : Identifiable(), Collection(), Readable<char>(), code( other.code )
    {
        code->withhold();
    }

    size_t LightString:: size() const noexcept
    {
        assert(0!=code);
        return code->size;
    }

    const char * LightString:: callSign() const noexcept
    {
        return CallSign;
    }

    const char & LightString:: operator[](const size_t indx) const noexcept
    {
        assert(0!=code);
        assert(indx>0);
        assert(indx<=code->size+1);
        return code->data[indx-1];
    }

    std::ostream & operator<<(std::ostream &os, const LightString &p)
    {
        assert(0!=p.code);
        assert(0!=p.code->data);
        os << p.code->data;
        return os;
    }


}

