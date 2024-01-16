#include "y/json/value.hpp"
#include "y/object.hpp"

namespace Yttrium
{
    namespace JSON
    {

        Value:: Value() noexcept : type(IsNull), impl(0) {}


        Value:: ~Value() noexcept
        {
            switch(type)
            {
                case IsTrue:
                case IsFalse:
                case IsNull:   assert(0==impl); break;
                case IsString: delete static_cast<String *>(impl); break;
                case IsArray:  delete static_cast<Array  *>(impl); break;
                case IsNumber:
                    Yttrium::Object::zrelease( static_cast<double *>(impl) );
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
        impl( Yttrium::Object::zacquire<double>() )
        {
            assert(0!=impl);
            *static_cast<double *>(impl) = x;
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

        Array:: Array(const Array &other) : Vector<Value>(other)
        {
        }

        Array & Array:: operator=(const Array &other)
        {
            Array tmp(other);
            swapWith(tmp);
            return *this;
        }

    }
    
}
