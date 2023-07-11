//! \file

#ifndef Yttrium_Config_Starting_Included
#define Yttrium_Config_Starting_Included 1


#include "y/config/starting.h"
#include "y/config/noexcept.hpp"

#define Y_DISABLE_COPY(CLASS)   private: CLASS(const CLASS &)               //!< no copy constructor
#define Y_DISABLE_ASSIGN(CLASS) private: CLASS & operator=(const CLASS &)   //!< no assign method

//! no copy/assign class
#define Y_DISABLE_COPY_AND_ASSIGN(CLASS) \
Y_DISABLE_ASSIGN(CLASS);\
Y_DISABLE_COPY(CLASS)

namespace Yttrium
{

    typedef ptrdiff_t unit_t; //!< alias

    //__________________________________________________________________________
    //
    //! C-stype swap
    //__________________________________________________________________________
    template <typename T>
    inline void Swap(T &a, T &b)
    {
        T t(a);
        a = b;
        b = t;
    }

    //__________________________________________________________________________
    //
    //! Coerce to mutable
    //__________________________________________________________________________
    template <typename T>
    inline T &Coerce(const T &args) noexcept
    {
        return (T&)args;
    }

    bool Die(const char *);                         //!< return true
    bool Say(const char *);                         //!< return false
    bool Good(const void *addr, const size_t size); //!< !(addr==NULL&&size>0) */

    namespace Core
    {
        extern const char Unknown[];         //!< "unknown"
        extern const char Success[];         //!< "success"
        extern const char Failure[];         //!< "failure"
        extern const char Warning[];         //!< "warning"
        extern const char Message[];         //!< "message"

        //______________________________________________________________________
        //
        //! helper to indent
        //______________________________________________________________________
        template <typename OSTREAM> inline
        OSTREAM & Indent(OSTREAM &os, size_t indent, const char ch = ' ') {
            while(indent-- > 0) os << ch;
            return os;
        }


        //______________________________________________________________________
        //
        //! helper to display arrays
        //______________________________________________________________________
        template <typename OSTREAM, typename T> inline
        OSTREAM & Display(OSTREAM &os, T *arr, const size_t num) {
            assert(Good(arr,num));
            os << '[';
            if(num>0)
            {
                os << arr[0];
                for(size_t i=1;i<num;++i) os << ';' << arr[i];
            }
            os << ']';
            return os;
        }

        //______________________________________________________________________
        //
        //! helper to safe string-like length
        //______________________________________________________________________
        template <typename T> static inline
        size_t Length(const T *arr) noexcept
        {
            if(0==arr)
                return 0;
            else
            {
                size_t count = 0;
                while(0 != *(arr++) ) ++count;
                return count;
            }
        }
    }

    //! mapping integer to class
    template <int N>
    struct Int2Type
    {
        static const int Value = N; //!< alias
    };

    //! mapping type to class
    template <typename T>
    struct Type2Type
    {
        typedef T Type; //!< alias
    };




}

#endif

