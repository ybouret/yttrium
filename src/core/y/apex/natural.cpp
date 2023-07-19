
#include "y/apex/natural.hpp"
#include "y/apex/n/cxx.hpp"
namespace Yttrium
{
    Y_SHALLOW_IMPL(TwoToThe);

    namespace Apex
    {

        Y_SHALLOW_IMPL(AsImpl);
        //----------------------------------------------------------------------
        //
        //
        // C++
        //
        //
        //----------------------------------------------------------------------
        const char * const Natural :: CallSign = Nexus::Proto::CallSign;

        
       

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
        impl( new Prototype( CONST_PROTO(other) ) )
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
            return CONST_PROTO(*this).bytes;
        }

        size_t Natural:: bits() const noexcept
        {
            return CONST_PROTO(*this).nbits;
        }

        Natural & Natural:: operator=(const Natural &other)
        {
            Prototype       &me = PROTO(*this);
            const Prototype &it = CONST_PROTO(other);
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
            PROTO(*this).ld64(qword);
            return *this;
        }

        Natural:: Natural(void *user,const AsImpl_&) noexcept :
        Number(),
        impl(user)
        {
            assert(0!=impl);
        }

        

        uint64_t Natural:: u64() const noexcept
        {
            return CONST_PROTO(*this).ls64();
        }

        
       
      
        //----------------------------------------------------------------------
        //
        //
        // Bitwise
        //
        //
        //----------------------------------------------------------------------
        Natural:: Natural(const TwoToThe_ &, const size_t p) :
        Number(),
        impl( new Prototype(p,AsShift) )
        {
        }

        void Natural:: shr() noexcept
        {
            PROTO(*this).shr();
        }

        bool Natural:: isEven() const noexcept
        {
            return 0 == (CONST_PROTO(*this).block.entry[0] & 0x1);
        }

        bool Natural:: isOdd() const noexcept
        {
            return 0 != (CONST_PROTO(*this).block.entry[0] & 0x1);
        }

      

    }

}


