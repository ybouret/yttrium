//! \file

#ifndef Y_JSON_Value_Included
#define Y_JSON_Value_Included 1

#include "y/string.hpp"
#include "y/sequence/vector.hpp"
#include "y/ptr/ark.hpp"

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
            Value() noexcept;           //!< IsNull
            Value(const String &);      //!< IsString
            Value(const char   *);      //!< IsString
            Value(const bool) noexcept; //!< Is[True|False]
            Value(const Number);        //!< IsNumber

            void  swapWith(Value &) noexcept;

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

        typedef ArkPtr<String,Pair> SharedPair;
        
        class Object
        {
        public:


        };

    }

}

#endif

