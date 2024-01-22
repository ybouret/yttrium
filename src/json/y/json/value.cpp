#include "y/json/value.hpp"
#include "y/object.hpp"
#include <cstring>
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace JSON
    {

        Y_SHALLOW_IMPL(AsArray);
        Y_SHALLOW_IMPL(AsObject);

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

        void Value:: swapWith(Value &other) noexcept
        {
            CoerceSwap(type,other.type);
            Swap(impl,other.impl);
        }

        Value:: Value(const AsArray_  &) :
        type(IsArray),
        impl( new Array() )
        {
        }

        Value:: Value(const AsObject_  &) :
        type(IsObject),
        impl( new Object() )
        {
        }

        std::ostream & Value::Indent(std::ostream &os, const size_t indent)
        {
            for(size_t i=indent;i>0;--i) os << "  ";
            return os;
        }


        template <>
        const Number & Value:: as<Number>() const noexcept
        {
            assert(IsNumber==type);
            return *static_cast<const Number *>(impl);
        }

        template <>
        const String & Value:: as<String>() const noexcept
        {
            assert(IsString==type);
            return *static_cast<const String *>(impl);
        }

        template <>
        const Array & Value:: as<Array>() const noexcept
        {
            assert(IsArray==type);
            return *static_cast<const Array *>(impl);
        }

        template <>
        const JSON::Object & Value:: as<JSON::Object>() const noexcept
        {
            assert(IsObject==type);
            return *static_cast<const Object *>(impl);
        }

        std::ostream &  Value:: display(std::ostream &os, const size_t indent) const
        {
            switch(type)
            {
                case IsNull:   os << "null";  break;
                case IsTrue:   os << "true";  break;
                case IsFalse:  os << "false"; break;
                case IsString: os << "\"" << as<String>() << "\""; break;
                case IsNumber: os << as<Number>(); break;
                case IsArray:  return as<Array>().display(os,indent);
                case IsObject: return as<Object>().display(os,indent);
            }
            return os;
        }

        std::ostream & operator<<(std::ostream &os, const Value &v)
        {
            return v.display(os,0);
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



        Array:: Array(const Array &other) :
        Identifiable(), Collection(), Yttrium::Vector<Value>(other)
        {
        }

        Array & Array:: operator=(const Array &other)
        {
            Array tmp(other);
            swapWith(tmp);
            return *this;
        }

        void Array:: add(Value &value)
        {
            {
                const Value nil;
                pushTail(nil);
            }
            tail().swapWith(value);
        }

        std::ostream & Array:: display(std::ostream &os, size_t indent) const
        {
            if(size()<=0)
            {
                os << "[]";
            }
            else
            {
                os << "[";
                indent += 1;
                (*this)[1].display(os,indent);
                for(size_t i=2;i<=size();++i)
                {
                    Value::Indent(os << ",\n",indent);
                    (*this)[i].display(os,indent);
                }
                os << " ]";
            }
            return os;
        }


    }


    namespace JSON
    {
        Pair:: ~Pair() noexcept
        {
        }

        

        Pair:: Pair(const String &str) :
        k(str),
        v()
        {
        }
        

        const String & Pair:: key() const noexcept
        {
            return k;
        }



    }

    namespace JSON
    {
        Object:: Object() : Pairs()
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

        const Value & Object:: operator[](const String &key) const
        {

            {
                const SharedPair *pp = search(key);
                if(0!=pp) return (**pp).v;
            }


            throw Specific::Exception("JSON::Object","no ['%s']", key.c_str());
        }

        Value & Object:: operator[](const String &key)
        {

            {
                SharedPair *pp = search(key);
                if(0!=pp) return (**pp).v;
            }

            Pair *p = new Pair(key);
            {
                const SharedPair tmp(p);
                if(!insert(tmp)) throw Specific::Exception("JSON::Object", "couldn't insert '%s'", key.c_str());
            }
            return p->v;

        }

        std::ostream & Object:: display(std::ostream &os, const size_t indent) const
        {

            os << '{';
            size_t num = size();
            size_t idx = 1;
            for(ConstIterator it=begin();it!=end();++it,++idx)
            {
                os << "\"" << it->key() << "\" : ";
                os << (*it)->v;
                if(idx<num) os << ',' << std::endl;
            }
            os << '}';
            return os;
        }

    }
    
}
