//! \file

#ifndef Y_String_Core_Included
#define Y_String_Core_Included 1

#include "y/counted.hpp"
#include "y/object.hpp"
#include "y/container/writable.hpp"
#include "y/container/iterator/writable-contiguous.hpp"
#include "y/sequence/interface.hpp"
#include "y/ostream-proto.hpp"
#include "y/type/capacity.hpp"
#include "y/memory/buffer/ro.hpp"

namespace Yttrium
{

    namespace Random { class Bits; }

    namespace Core
    {
        //______________________________________________________________________
        //
        //
        //! base class for strings
        //
        //______________________________________________________________________
        class StringCommon : public Object, public Counted, public Memory::ReadOnlyBuffer
        {
        public:
            static const size_t MinChars = 31;                       //!< minimal Chars
            static size_t BlocksFor(const size_t numChars) noexcept; //!< 1+Max(numChar,Minchar)

            explicit StringCommon() noexcept; //!< setup
            virtual ~StringCommon() noexcept; //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(StringCommon);
        };

        template <typename> class String;

        template <typename T> String<T> operator+(const String<T> &, const String<T> &); //!< forward declaration
        template <typename T> String<T> operator+(const String<T> &, const T);           //!< forward declaration
        template <typename T> String<T> operator+(const String<T> &, const T *);         //!< forward declaration
        template <typename T> String<T> operator+(const T, const String<T> &);           //!< forward declaration
        template <typename T> String<T> operator+(const T*, const String<T> &);          //!< forward declaration


        //______________________________________________________________________
        //
        //
        //
        //! Generic String
        //
        //
        //______________________________________________________________________
        template <typename T>
        class String :
        public StringCommon,
        public Sequence<T>,
        public Writable<T>,
        public WritableContiguous<T>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            Y_ARGS_DECL(T,Type); //!< aliases

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            String();                                  //!< setup emtpy
            virtual ~String() noexcept;                //!< cleanup
            String(const String &);                    //!< copy
            String & operator=( const String &);       //!< assign
            String(const T);                           //!< setup from char
            String(const T *, const size_t);           //!< setup from given chars
            String(const T *);                         //!< setup from chars
            String(const String &, const size_t);      //!< copy with extra capacity


            //! make a (prefilled) String with minimal chars
            explicit String(const size_t       minLength,
                            const AsCapacity_ &,
                            const bool         setLength);


            //! display, legacy style
            friend inline std::ostream & operator<<(std::ostream &os,
                                                    const String &s)
            {
                os << s();
                return os;
            }

            //! generate random little string
            explicit String(const size_t len, Random::Bits &ran, const T lower = 'a', const T upper='z');

            //__________________________________________________________________
            //
            //
            // Access Methods
            //
            //__________________________________________________________________
            virtual ConstType &  operator[](const size_t index) const noexcept; //!< const access
            virtual Type      &  operator[](const size_t index)       noexcept; //!< access
            virtual size_t       size()                         const noexcept; //!< num of chars
            virtual size_t       capacity()                     const noexcept; //!< maximal number of chars
            ConstType         *  operator()(void)               const noexcept; //!< legacy access
            virtual const char * callSign()                     const noexcept; //!< String<CH>
            virtual const void * ro_addr()                      const noexcept; //!< address of first char
            virtual size_t       measure()                      const noexcept; //!< size() * sizeof(CH)

            
            virtual void pushHead(ParamType);
            virtual void pushTail(ParamType);
            virtual void popTail()  noexcept;
            virtual void popHead()  noexcept;

            void         reverse() noexcept; //!< reverse string

            //__________________________________________________________________
            //
            //
            // appending
            //
            //__________________________________________________________________
            void pushAtTail(const T *, const size_t); //!< push block at tail
            String & operator<<(const T       );      //!< helper
            String & operator<<(const T      *);      //!< helper
            String & operator<<(const String &);      //!< helper

            //__________________________________________________________________
            //
            //
            // prepending
            //
            //__________________________________________________________________
            void pushAtHead(const T *, const size_t); //!< push block at head
            String & operator>>(const T       );    //!< helper
            String & operator>>(const T      *);    //!< helper
            String & operator>>(const String &);    //!< helper

            //__________________________________________________________________
            //
            //
            // additions
            //
            //__________________________________________________________________
            friend String<T> operator+<>(const String<T> &, const String<T> &); //!< declaration
            friend String<T> operator+<>(const String &, const T      *);       //!< declaration
            friend String<T> operator+<>(const T      *, const String &);       //!< declaration
            friend String<T> operator+<>(const String &, const T       );       //!< declaration
            friend String<T> operator+<>(const T       , const String &);       //!< declaration

            String & operator+=(const String &); //!< in place addition
            String & operator+=(const T      *); //!< in place addition
            String & operator+=(const T       ); //!< in place addition

            //__________________________________________________________________
            //
            //! clear
            //__________________________________________________________________
            void clear() noexcept;

            //__________________________________________________________________
            //
            //! no-throw swap
            //__________________________________________________________________
            void swapWith(String &s) noexcept;


        private:
            class Code;
            Code *code;
            String(const T *,const size_t, const T *, const size_t);
            virtual const T *   getBaseForward() const noexcept;
            virtual const T *   getLastForward() const noexcept;
            virtual const T *   getBaseReverse() const noexcept;
            virtual const T *   getLastReverse() const noexcept;
            virtual ConstType & getHead()        const noexcept;
            virtual ConstType & getTail()        const noexcept;
        };
    }
}

#endif

