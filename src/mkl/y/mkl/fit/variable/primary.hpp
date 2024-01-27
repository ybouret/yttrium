
//! \file

#ifndef Y_Fit_Variable_Primary_Included
#define Y_Fit_Variable_Primary_Included 1

#include "y/mkl/fit/variable/interface.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace Fit
        {
            //__________________________________________________________________
            //
            //
            //
            //! Variable with primary index
            //
            //
            //__________________________________________________________________
            class PrimaryVariable : public Variable
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                
                //! setup
                template <typename ID> inline
                explicit PrimaryVariable(const ID &i, const size_t j) :Variable(i), indx( CheckIndex(*this,j) ) {}
                
                //! cleanup
                virtual ~PrimaryVariable() noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(PrimaryVariable);
                static  size_t CheckIndex(const String &, const size_t); //!< positive index
                virtual size_t idx() const noexcept;                     //!< indx
                virtual void   dsp(std::ostream &) const;                //!< display index
                const size_t indx;                                       //!< value
            };

        }

    }

}

#endif

