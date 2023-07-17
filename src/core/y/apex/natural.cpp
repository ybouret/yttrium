
#include "y/apex/natural.hpp"
#include "y/apex/n/proto.hpp"
namespace Yttrium
{
    namespace Apex
    {


        Y_SHALLOW_IMPL(AsImpl);


        typedef Proto<uint64_t,uint32_t> Prototype;

#define PROTO(ADDR)       (*static_cast<Prototype *>(ADDR))
#define CONST_PROTO(ADDR) (*static_cast<const Prototype *>(ADDR))

        Natural:: ~Natural() noexcept
        {
            assert(0!=impl);
            delete static_cast<Prototype*>(impl);
            impl = 0;
        }

        Natural:: Natural() :
        Number(),
        impl( new Prototype(0,AsCapacity) )
        {
        }

        Natural:: Natural(const uint64_t qw) :
        Number(),
        impl( new Prototype(qw) )
        {
        }

        

        Natural:: Natural(const Natural &other) :
        Number(),
        impl( new Prototype( CONST_PROTO(other.impl) ) )
        {
        }

        Natural:: Natural(const size_t n, Random::Bits &ran) :
        Number(),
        impl( new Prototype(n,ran) )
        {

        }

        void Natural:: xch(Natural &other) noexcept
        {
            Swap(impl,other.impl);
        }

        size_t Natural:: bytes() const noexcept
        {
            return CONST_PROTO(impl).bytes;
        }

        size_t Natural:: bits() const noexcept
        {
            return CONST_PROTO(impl).nbits;
        }

        Natural & Natural:: operator=(const Natural &other)
        {
            Prototype       &me = PROTO(impl);
            const Prototype &it = CONST_PROTO(other.impl);
            if(!me.couldSteal(it))
            {
                Prototype *clone = new Prototype(it);
                delete    &me;
                impl = clone;
            }
            return *this;
        }


        Natural & Natural:: operator=(const uint64_t qword) noexcept
        {
            PROTO(impl).ld64(qword);
            return *this;
        }

        Natural:: Natural(void *user,const AsImpl_&) noexcept :
        Number(),
        impl(user)
        {
            assert(0!=impl);
        }

        std::ostream & Natural:: printHex(std::ostream &os) const
        {
            CONST_PROTO(impl).printHex(os);
            return os;
        }

        Natural operator+(const Natural &lhs, const Natural &rhs)
        {
            return Natural(Prototype::Add( CONST_PROTO(lhs.impl), CONST_PROTO(rhs.impl)),AsImpl);
        }

        Natural operator+(const Natural &lhs, const uint64_t rhs)
        {
            return Natural(Prototype::Add(CONST_PROTO(lhs.impl),rhs),AsImpl);
        }

        Natural operator+(const uint64_t lhs, const Natural &rhs)
        {
            return Natural(Prototype::Add(CONST_PROTO(rhs.impl),lhs),AsImpl);
        }

        Natural & Natural:: operator+=(const uint64_t rhs)
        {
            Natural res( Prototype::Add(CONST_PROTO(impl),rhs),AsImpl);
            xch(res);
            return *this;
        }

        Natural & Natural:: operator+=(const Natural &rhs)
        {
            Natural res( Prototype::Add(CONST_PROTO(impl),CONST_PROTO(rhs.impl)), AsImpl);
            xch(res);
            return *this;
        }

        Natural & Natural:: operator++()
        {
            Natural res( Prototype::Add1(CONST_PROTO(impl)), AsImpl);
            xch(res);
            return *this;
        }

        Natural   Natural:: operator++(int)
        {
            const Natural old(*this);
            {
                Natural res( Prototype::Add1(CONST_PROTO(impl)), AsImpl);
                xch(res);
            }
            return old;
        }
        

        Natural:: operator uint64_t() const noexcept
        {
            return CONST_PROTO(impl).ls64();
        }



    }

}


