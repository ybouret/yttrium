//! \file

#ifndef Y_String_Core_Included
#define Y_String_Core_Included 1

#include "y/counted.hpp"
#include "y/object.hpp"
#include "y/container/writable.hpp"
#include "y/ostream-proto.hpp"

namespace Yttrium
{

    namespace Core
    {
        class StringCommon : public Object, public Counted
        {
        public:
            static const size_t MinChars = 31;
            static size_t BlocksFor(const size_t numChars) noexcept;

            virtual ~StringCommon() noexcept;

        protected:
            explicit StringCommon() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(StringCommon);
        };

        //______________________________________________________________________
        //
        //
        //
        //! Generic String
        //
        //
        //______________________________________________________________________
        template <typename T>
        class String : public StringCommon, public Writable<T>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            Y_ARGS_EXPOSE(T); //!< aliases

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit String();                   //!< setup emtpy
            virtual ~String() noexcept;          //!< cleanup
            String(const String &);              //!< copy
            String & operator=( const String &); //!< assign

            String(const T);
            String(const T *, const size_t);
            String(const T *);

            friend inline std::ostream & operator<<(std::ostream &os,
                                                    const String &s)
            {
                os << s();
                return os;
            }

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual ConstType & operator[](const size_t index) const noexcept;
            virtual Type      & operator[](const size_t index)       noexcept;
            virtual size_t      size()                         const noexcept;
            virtual size_t      capacity()                     const noexcept;
            ConstType         * operator()(void)               const noexcept;



        private:
            class Code;
            Code *code;
        };
    }
}

#endif

