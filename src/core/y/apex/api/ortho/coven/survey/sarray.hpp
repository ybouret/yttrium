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

