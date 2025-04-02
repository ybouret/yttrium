//! \file

#ifndef Y_Apex_Ortho_Coven_SArray_Included
#define Y_Apex_Ortho_Coven_SArray_Included 1

#include "y/apex/api/ortho/vector.hpp"
#include "y/apex/api/natural-part.hpp"

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
                    ncof(0),
                    nrm1(0),
                    next(0),
                    prev(0)
                    {
                        setup();
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
                        os << self << " // |#" << arr.ncof << "| =" << arr.nrm1;
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
                                     const SArray * const rhs)  
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

                        switch( Sign::Of(L.nrm1,R.nrm1) )
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
                    const apn    nrm1; //!< ||*this||_1
                    SArray *     next; //!< for list
                    SArray *     prev; //!< for list



                private:
                    Y_DISABLE_COPY_AND_ASSIGN(SArray);
                    inline void setup()
                    {
                        {
                            const Readable<T> &self = *this;
                            for(size_t i=self.size();i>0;--i)
                            {
                                const apn &n = NaturalPart::Of( self[i] );
                                if(n->bits>0) {
                                    Coerce(ncof)++;
                                    Coerce(nrm1) += n;
                                }
                            }
                        }
                        assert(ncof>=2);
                    }
                };
            }

        }

    }

}

#endif

