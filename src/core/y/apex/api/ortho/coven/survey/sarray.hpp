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
                template <typename T>
                class SArray : public Quantized, public CxxArray<T,Memory::Dyadic>
                {
                public:
                    typedef CxxArray<T,Memory::Dyadic> SArrayType;

                    explicit SArray(const Vector &v) :
                    SArrayType(CopyOf,v),
                    ncof(CountNonZero(*this)),
                    next(0),
                    prev(0)
                    {
                    }

                    virtual ~SArray() noexcept
                    {
                    }

                    inline friend
                    std::ostream & operator<<(std::ostream &os, const SArray &arr)
                    {
                        const SArrayType &self = arr;
                        os << self << " //#" << arr.ncof;
                        return os;
                    }

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

                    const size_t ncof;
                    SArray *     next;
                    SArray *     prev;



                private:
                    Y_DISABLE_COPY_AND_ASSIGN(SArray);
                };
            }

        }

    }

}

#endif

