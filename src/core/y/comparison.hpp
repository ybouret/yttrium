
//! \file

#ifndef Y_Comparison_Included
#define Y_Comparison_Included 1

#include "y/type/signs.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //! Comparison function
    //
    //__________________________________________________________________________
    struct Comparison
    {

        //______________________________________________________________________
        //
        //! C style increasing
        //______________________________________________________________________
        template <typename T> static inline
        int Increasing(const T &lhs, const T &rhs)
        {
            return lhs < rhs ? -1 : ( rhs < lhs ? 1 : 0);
        }

        //______________________________________________________________________
        //
        //! C style increasing abs
        //______________________________________________________________________
        template <typename T> static inline
        int IncreasingAbs(const T &lhs, const T &rhs)
        {
            const T zero(0);
            const T l = lhs <= zero ? -lhs : lhs;
            const T r = rhs <= zero ? -rhs : rhs;
            return Increasing(l,r);
        }

        //______________________________________________________________________
        //
        //! C style decreasing
        //______________________________________________________________________
        template <typename T> static inline
        int Decreasing(const T &lhs, const T &rhs)
        {
            return lhs < rhs ? 1 : ( rhs < lhs ? -1 : 0);
        }

        //______________________________________________________________________
        //
        //! C++ style increasing
        //______________________________________________________________________
        template <typename T> static inline
        SignType CxxIncreasing(const T &lhs, const T &rhs)
        {
            return Sign::Of(lhs,rhs);
        }

        //______________________________________________________________________
        //
        //! C++ style decreasing
        //______________________________________________________________________
        template <typename T> static inline
        SignType CxxDecreasing(const T &lhs, const T &rhs)
        {
            return Sign::Of(rhs,lhs);
        }

        //______________________________________________________________________
        //
        //! Check Increasing ARRAY (readable)
        //______________________________________________________________________
        template <typename ARRAY> static inline
        bool IsIncreasing(ARRAY &arr, const size_t num)
        {
            if(num<=1) return true;
            for(size_t i=1;i<num;++i)
            {
                if(arr[i+1]<arr[i]) return false;
            }
            return true;
        }

        //______________________________________________________________________
        //
        //! Check Decreasing ARRAY (readable)
        //______________________________________________________________________
        template <typename ARRAY> static inline
        bool IsDecreasing(ARRAY &arr, const size_t num)
        {
            if(num<=1) return true;
            for(size_t i=1;i<num;++i)
            {
                if(arr[i+1]>arr[i]) return false;
            }
            return true;
        }



    };

    //__________________________________________________________________________
    //
    //
    //! Making a comparator class
    //
    //__________________________________________________________________________
    class IncreasingComparator
    {
    public:
        IncreasingComparator()  noexcept; //!< setup
        ~IncreasingComparator() noexcept; //!< cleanup

        //! generic interface
        template <typename T> inline
        SignType operator()(T &lhs, T &rhs) const { return Comparison::CxxIncreasing<T>(lhs,rhs); }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(IncreasingComparator);
    };

    //__________________________________________________________________________
    //
    //
    //! Making a comparator class
    //
    //__________________________________________________________________________
    class DecreasingComparator
    {
    public:
        DecreasingComparator()  noexcept; //!< setup
        ~DecreasingComparator() noexcept; //!< cleanup

        //! generic interface
        template <typename T> inline
        SignType operator()(T &lhs, T &rhs) const { return Comparison::CxxDecreasing<T>(lhs,rhs); }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(DecreasingComparator);
    };
    

}

#endif

