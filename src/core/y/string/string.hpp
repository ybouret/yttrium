//! \file

#ifndef Y_String_Core_Included
#define Y_String_Core_Included 1

#include "y/counted.hpp"
#include "y/object.hpp"
#include "y/container/writable.hpp"

namespace Yttrium
{

    namespace Core
    {
        //______________________________________________________________________
        //
        //
        //
        //! Generic String
        //
        //
        //______________________________________________________________________
        template <typename T>
        class String : public Object, public Counted, public Writable<T>
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

            void append(const T *arr, const size_t num);
            
            String & operator<<(const T);
            String & operator<<(const T *);
            String & operator<<(const String &);


        private:
            class Impl;
            Impl *impl;
        };
    }
}

#endif

