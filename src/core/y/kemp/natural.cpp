#include "y/kemp/natural.hpp"
#include "y/kemp/element.hpp"
#include "y/type/nullify.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        Ops                Natural:: Strategy = Ops64_32;
        const char * const Natural:: CallSign = "apn";

        Y_SHALLOW_IMPL(AsElement);

        Natural:: Natural(Element * const el, const AsElement_ &) noexcept :
        code(el)
        {
            assert(0!=code);
        }

        void Natural:: make(Element *el) noexcept
        {
            assert(0!=el);
            assert(0!=code);
            Swap(code,el);
            delete el;
        }


        Element & Natural:: operator*() const noexcept
        {
            return *code;
        }

        Natural:: ~Natural() noexcept
        {
            assert(0!=code);
            Nullify(code);
        }

        Natural:: Natural() :
        Number(),
        code( new Element(0,AsCapacity) )
        {
        }

        Natural:: Natural(const Natural &n) :
        Number(),
        code( new Element( *n.code) )
        {}

        void Natural:: xch(Natural &n) noexcept
        {
            Swap(code,n.code);
        }

        Natural & Natural:: operator=(const Natural &n)
        {
            {Natural _(n); xch(_);}
            return *this;
        }

        Natural & Natural:: operator=(const uint64_t qw)
        {
            {Natural _(qw); xch(_);}
            return *this;
        }

        Natural::Natural(const uint64_t qw) : Number(), code( new Element(qw,ToNum64) )
        {
        }

        Natural:: Natural(const size_t nbits, Random::Bits &ran) :
        Number(),
        code( new Element(nbits,ran) )
        {

        }

        Natural:: Natural(const TwoToThe_ &_, const size_t ibit) :
        Number(),
        code( new Element(_,ibit) )
        {
        }


        size_t Natural:: bits()   const noexcept
        {
            assert(0!=code);
            return code->bits;
        }

        size_t Natural:: size()   const noexcept
        {
            assert(0!=code);
            return code->bytes.positive;
        }

        uint8_t Natural:: operator[](const size_t i) const noexcept
        {
            assert(0!=code);
            const Assembly<uint8_t> &data = code->get<uint8_t>(); assert(i<data.positive);
            return data.item[i];
        }


    }

}

