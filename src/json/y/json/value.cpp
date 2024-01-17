#include "y/json/value.hpp"
#include "y/object.hpp"
#include <cstring>

namespace Yttrium
{
    namespace JSON
    {

        Value:: Value() noexcept : type(IsNull), impl(0) {}


        Value:: ~Value() noexcept
        {
            nullify();
        }

        void Value:: nullify() noexcept
        {

            switch(type)
            {
                case IsTrue:
                case IsFalse:
                case IsNull:   assert(0==impl); break;
                case IsString: delete static_cast<String  *>(impl); break;
                case IsArray:  delete static_cast<Array   *>(impl); break;
                case IsObject: delete static_cast<Object  *>(impl); break;
                case IsNumber: Yttrium::Object::zrelease( static_cast<Number *>(impl) ); break;
            }
            Coerce(type) = IsNull;
            impl         = 0;
        }

        Value:: Value(const Value &other) :
        type(other.type),
        impl(0)
        {
            switch(type)
            {
                case IsTrue:
                case IsFalse:
                case IsNull:   assert(0==impl); break;
                case IsString: impl = new String( *static_cast<const String  *>(other.impl) ); break;
                case IsArray:  impl = new Array(  *static_cast<const Array   *>(other.impl) ); break;
                case IsObject: impl = new Object( *static_cast<const Object  *>(other.impl) ); break;
                case IsNumber:
                    impl = Yttrium::Object::zacquire<Number>();
                    memcpy(impl,other.impl,sizeof(Number));
                    break;
            }
        }

        Value:: Value(const String &s) :
        type(IsString),
        impl( new String(s) )
        {
        }

        Value:: Value(const char *s) :
        type(IsString),
        impl( new String(s) )
        {
        }

        Value:: Value(const bool flag) noexcept :
        type(flag ? IsTrue : IsFalse),
        impl(0)
        {
        }


        Value:: Value(const double x) :
        type(IsNumber),
        impl( Yttrium::Object::zacquire<Number>() )
        {
            assert(0!=impl);
            *static_cast<Number *>(impl) = x;
        }

    }

    namespace JSON
    {
        Array:: ~Array() noexcept
        {
        }


        Array:: Array() noexcept
        {
        }

        Array:: Array(const size_t n) :
        Vector<Value>(n,AsCapacity)
        {
        }

        Array:: Array(const Array &other) :
        Identifiable(), Collection(), Vector<Value>(other)
        {
        }

        Array & Array:: operator=(const Array &other)
        {
            Array tmp(other);
            swapWith(tmp);
            return *this;
        }

    }

    namespace JSON
    {
        Object:: Object(const size_t n) : Pairs(n,AsCapacity)
        {
        }

        Object:: ~Object() noexcept
        {
        }

        Object:: Object(const Object &other) : Identifiable(), Collection(), Pairs(other)
        {
        }

        Object & Object:: operator=(const Object &other)
        {
            Pairs tmp(other);
            return *this;
        }
    }
    
}
