
#include "y/ptr/cstr.hpp"
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
            rsize(r)
            {
            }

            inline virtual ~Metrics() noexcept
            {
            }

            const size_t lsize;
            const size_t rsize;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Metrics);
        };

    }

    class CStrPtr:: Code : public Object, public Counted, public Metrics, public WadType
    {
    public:
        explicit Code(const char *lhs) :
        Object(),
        Counted(),
        Metrics( StringLength(lhs), 0),
        WadType( lsize+1 ),
        data( static_cast<char *>(workspace) )
        {
            memcpy(workspace,lhs,lsize);
        }

        explicit Code(const char *lhs, const char *rhs) :
        Object(),
        Counted(),
        Metrics( StringLength(lhs), StringLength(rhs) ),
        WadType( lsize+rsize+1 ),
        data( static_cast<char *>(workspace) )
        {
            memcpy(workspace,lhs,lsize);
            memcpy(static_cast<char*>(workspace)+lsize,rhs,rsize);
        }

        virtual ~Code() noexcept
        {
            memset(workspace,0,allocated);
        }

        const char * const data;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Code);
    };



    CStrPtr:: CStrPtr(const char *lhs) : code( new Code(lhs) )
    {
        code->withhold();
    }

    CStrPtr:: CStrPtr(const char *lhs, const char *rhs) : code( new Code(lhs,rhs) )
    {
        code->withhold();
    }


    CStrPtr:: ~CStrPtr() noexcept
    {
        assert(0!=code);
        if(code->liberate()) delete code;
        code = 0;
    }

    CStrPtr:: CStrPtr(const CStrPtr &other) noexcept : code( other.code )
    {
        code->withhold();
    }

    const char * CStrPtr:: operator()(void) const noexcept
    {
        assert(0!=code);
        return code->data;
    }

    std::ostream & operator<<(std::ostream &os, const CStrPtr &p)
    {
        assert(0!=p.code);
        assert(0!=p.code->data);
        os << p.code->data;
        return os;
    }


}

