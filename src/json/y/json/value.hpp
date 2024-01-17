//! \file

#ifndef Y_JSON_Value_Included
#define Y_JSON_Value_Included 1

#include "y/string.hpp"
#include "y/sequence/vector.hpp"
#include "y/ptr/ark.hpp"
#include "y/associative/hash/set.hpp"

namespace Yttrium
{

    namespace JSON
    {

        enum Type
        {
            IsString,
            IsNumber,
            IsNull,
            IsTrue,
            IsFalse,
            IsArray,
            IsObject
        };

        typedef double Number;

        class Value
        {
        public:
            Value(const Value &);
            virtual ~Value() noexcept;  //!< cleanup
            Value & operator=(const Value &); //!< copy
            
            Value() noexcept;           //!< IsNull
            Value(const String &);      //!< IsString
            Value(const char   *);      //!< IsString
            Value(const bool) noexcept; //!< Is[True|False]
            Value(const Number);        //!< IsNumber

            void  swapWith(Value &) noexcept;
            void  nullify() noexcept;
            
            const Type type;
        private:
            void       *impl;
        };


        class Array : public Vector<Value>
        {
        public:
            explicit Array() noexcept;
            explicit Array(const size_t n);
            explicit Array(const Array &);
            virtual ~Array() noexcept;
            Array & operator=(const Array &other);
        };

        class Pair : public Yttrium::Object, public Counted
        {
        public:
            virtual ~Pair() noexcept;
            explicit Pair(const String &, const Value &);

            const String & key() const noexcept;

            const String name;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pair);
        };

        typedef ArkPtr<String,Pair>        SharedPair;
        typedef HashSet<String,SharedPair> Pairs;

        class Object : public Pairs
        {
        public:
            explicit Object(const size_t n=0);
            virtual ~Object() noexcept;
            explicit Object(const Object &);
            Object & operator=(const Object &);
            
        };

    }

}

#endif

