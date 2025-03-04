//! \file

#ifndef Y_Apex_Ortho_Coven_SArray_Included
#define Y_Apex_Ortho_Coven_SArray_Included 1

#include "y/apex/api/ortho/vector.hpp"
#include "y/apex/api/count-non-zero.hpp"

namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {
            namespace Coven
            {
                //______________________________________________________________
                //
                //
                //
                //! Survey Array
                //
                //
                //______________________________________________________________
                template <typename T>
                class SArray : public Quantized, public CxxArray<T,Memory::Dyadic>
                {
                public:
                    //__________________________________________________________
                    //
                    //
                    // Definitions
                    //
                    //__________________________________________________________
                    typedef CxxArray<T,Memory::Dyadic> SArrayType; //!< alias

                    //__________________________________________________________
                    //
                    //
                    // C++
                    //
                    //__________________________________________________________

                    //! setup from vector
                    explicit SArray(const Vector &v) :
                    SArrayType(CopyOf,v),
                    ncof(CountNonZero(*this)),
                    next(0),
                    prev(0)
                    {
                    }

                    //! cleanup
                    virtual ~SArray() noexcept
                    {
                    }

                    //! display
                    inline friend
                    std::ostream & operator<<(std::ostream &os, const SArray &arr)
                    {
                        const SArrayType &self = arr;
                        os << self << " //#" << arr.ncof;
                        return os;
                    }

                    //__________________________________________________________
                    //
                    //
                    // Methods
                    //
                    //__________________________________________________________

                    //! extended lexicographic comparison
                    static inline
                    SignType Compare(const SArray * const lhs,
                                     const SArray * const rhs) noexcept
                    {
                        assert(0!=lhs);
                        assert(0!=rhs);
                        assert(lhs->size() == rhs->size() );

                        const SArray &L = *lhs;
                        const SArray &R = *rhs;
                        switch( Sign::Of(L.ncof,R.ncof) )
                        {
                            case Negative: return Negative;
                            case Positive: return Positive;
                            case __Zero__: break;
                        }

                        const size_t n = L.size();
                        for(size_t i=1;i<=n;++i)
                        {
                            switch( T::Compare(L[i],R[i]) )
                            {
                                case Negative: return Negative;
                                case __Zero__: continue;
                                case Positive: return Positive;
                            }
                        }
                        return __Zero__;

                    }

                    //__________________________________________________________
                    //
                    //
                    // Members
                    //
                    //__________________________________________________________
                    const size_t ncof; //!< number of coefficients
                    SArray *     next; //!< for list
                    SArray *     prev; //!< for list



                private:
                    Y_DISABLE_COPY_AND_ASSIGN(SArray);
                };
            }

        }

    }

}

#endif

