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

        //______________________________________________________________________
        //
        //
        //
        //! Type of Value
        //
        //
        //______________________________________________________________________
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

        typedef double Number;     //!< alias
        Y_SHALLOW_DECL(AsArray);   //!< alias
        Y_SHALLOW_DECL(AsObject);  //!< alias

        //______________________________________________________________________
        //
        //
        //
        //! Variant Value
        //
        //
        //______________________________________________________________________
        class Value
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~Value() noexcept;        //!< cleanup
            Value(const Value &);             //!< copy
            Value & operator=(const Value &); //!< assign
            
            Value() noexcept;           //!< IsNull
            Value(const String &);      //!< IsString
            Value(const char   *);      //!< IsString
            Value(const bool) noexcept; //!< Is[True|False]
            Value(const Number);        //!< IsNumber

            Value(const AsArray_  &);   //!< IsArray
            Value(const AsObject_ &);   //!< IsObject

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void  swapWith(Value &) noexcept; //!< swap contents
            void  nullify() noexcept;         //!< clear content

            std::ostream &        display(std::ostream &os) const; //!< display

            Y_OSTREAM_PROTO(Value); //!< default display

            template <typename T> const T & as() const noexcept; //!< access data

            //! access data, const
            template <typename T> inline
            T & as() noexcept
            {
                const Value &self = *this;
                const T     &cstv = self.as<T>();
                return Coerce(cstv);
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Type type;  //!< type
        private:
            void       *impl; //!< opaque data

        };

        //______________________________________________________________________
        //
        //
        //
        //! Array = Vector of Values
        //
        //
        //______________________________________________________________________
        class Array : public Vector<Value>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Array()     noexcept;         //!< setup
            virtual ~Array()     noexcept;         //!< cleanup
            explicit Array(const Array &);         //!< copy
            Array & operator=(const Array &other); //!< assign

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void add(Value &value); //!< steal value, pushed
            std::ostream & display(std::ostream &) const; //!< display

        };


        //______________________________________________________________________
        //
        //
        //
        //! Pair key/value for Object
        //
        //
        //______________________________________________________________________
        class Pair : public Yttrium::Object, public Counted
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Pair(const String &); //!< setup
            virtual ~Pair() noexcept;      //!< cleanup

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            const String & key() const noexcept; //!< get key

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const String k; //!< key
            Value        v; //!< value
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pair);
        };

        typedef ArkPtr<String,Pair>        SharedPair; //!< alias
        typedef HashSet<String,SharedPair> Pairs;      //!< alias


        //______________________________________________________________________
        //
        //
        //
        //! Object = Set of SharedPairs
        //
        //
        //______________________________________________________________________
        class Object : public Pairs
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Object();                  //!< create
            virtual ~Object() noexcept;         //!< cleanup
            explicit Object(const Object &);    //!< copy
            Object & operator=(const Object &); //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            std::ostream & display(std::ostream &) const; //!< display

            Value &       operator[](const String &key);       //!< get/create (null) Value
            const Value & operator[](const String &key) const; //!< get existing Value

        };

    }

}

#endif

