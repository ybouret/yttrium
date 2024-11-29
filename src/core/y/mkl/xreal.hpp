
//! \file

#ifndef Y_Type_XReal_Included
#define Y_Type_XReal_Included 1

#include "y/type/signs.hpp"
#include "y/config/shallow.hpp"
#include <iostream>

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //! global output flag
    //
    //__________________________________________________________________________
    struct XRealOutput
    {
        //! output modes
        enum ModeType
        {
            Default, //!< mantissa^exponent
            Compact  //!< converted to real
        };
        
        static ModeType Mode; //!< global value
    };

    Y_SHALLOW_DECL(Raised); //!< helper to make arbitrary high/low XReal

    //! helper to implement operators
#define Y_XREAL_BINARY(OP,CALL) \
inline friend XReal   operator OP (const XReal &lhs, const XReal &rhs) noexcept { return CALL(lhs,rhs); }                            \
inline friend XReal   operator OP (const T      lhs, const XReal &rhs) noexcept { const XReal _(lhs); return CALL(_,rhs); }          \
inline friend XReal   operator OP (const XReal &lhs, const T      rhs) noexcept { const XReal _(rhs); return CALL(lhs,_); }          \
inline        XReal & operator OP##=(const XReal &rhs)                 noexcept { return (*this=CALL(*this,rhs)); }                  \
inline        XReal & operator OP##=(const T      rhs)                 noexcept { const XReal _(rhs); return (*this=CALL(*this,_)); }



    //! helper to implement friend comparisons
#define Y_XREAL_CMP(OP,RESULT) \
inline friend bool operator OP (const XReal &lhs, const XReal &rhs) noexcept { return Compare(lhs,rhs) RESULT; } \
inline friend bool operator OP (const XReal &lhs, const T      rhs) noexcept { return Compare(lhs,rhs) RESULT; } \
inline friend bool operator OP (const T      lhs, const XReal &rhs) noexcept { return Compare(lhs,rhs) RESULT; }

    //__________________________________________________________________________
    //
    //
    //! Extended Real
    //
    //__________________________________________________________________________
    template <typename T>
    class XReal
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        typedef T Type; //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        XReal() noexcept;                                //!< zero
        XReal(const T x);                                //!< decompose
        XReal(const XReal &) noexcept;                   //!< copy
        XReal & operator=(const XReal &) noexcept;       //!< assign
        XReal & operator=(const T);                      //!< decompose/assign
        ~XReal() noexcept;                               //!< cleanup
        XReal(const T x, const Raised_ &, const int xp); //!< x * radix&xp
        
        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        //! display
        inline friend std::ostream &operator<<(std::ostream &os, const XReal &xr)
        { xr.display(os); return os; }

        //! retrieveing floating point value
        operator T() const;

        void  ldz()         noexcept;   //!< in place set to zero
        void  neg()         noexcept;   //!< in place mantissa sign change
        XReal abs()   const noexcept;   //!< absolute value
        XReal sqrt()  const;            //!< square root
        T     log()   const;            //!< log
        T     log10() const;            //!< log10
        XReal pow(const T alpha) const; //!< this^alpha

        //______________________________________________________________________
        //
        //
        // operations
        //
        //______________________________________________________________________

#ifndef DOXYGEN_SHOULD_SKIP_THIS
        Y_XREAL_BINARY(*,Mul)
        Y_XREAL_BINARY(/,Div)
        Y_XREAL_BINARY(+,Add)
        Y_XREAL_BINARY(-,Sub)
#endif

        XReal operator+() const noexcept; //!< unary +
        XReal operator-() const noexcept; //!< unary -


        //______________________________________________________________________
        //
        //
        // comparisons
        //
        //______________________________________________________________________
        static SignType Compare(const XReal &lhs, const XReal &rhs) noexcept; //!< generic comparison, C++ style
        static SignType Compare(const XReal &lhs, const T      rhs) noexcept; //!< alias
        static SignType Compare(const T      lhs, const XReal &rhs) noexcept; //!< alias


        Y_XREAL_CMP(==,  == __Zero__ )
        Y_XREAL_CMP(!=,  != __Zero__ )
        Y_XREAL_CMP(<,   == Negative )
        Y_XREAL_CMP(>,   == Positive )
        Y_XREAL_CMP(<=,  != Positive )
        Y_XREAL_CMP(>=,  != Negative )


        //______________________________________________________________________
        //
        //
        // members
        //
        //______________________________________________________________________
        const int exponent; //!< exponent
        const T   mantissa; //!< mantissa

    private:
        explicit XReal(const int, const T) noexcept;
        static XReal Mul(const XReal &, const XReal &) noexcept;
        static XReal Div(const XReal &, const XReal &);
        static XReal Add(const XReal &, const XReal &);
        static XReal Sub(const XReal &, const XReal &);
        void   display(std::ostream &) const;
    };

}

#endif

