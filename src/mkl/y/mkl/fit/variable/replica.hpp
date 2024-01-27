

//! \file

#ifndef Y_Fit_Variable_Replica_Included
#define Y_Fit_Variable_Replica_Included 1

#include "y/mkl/fit/variable/primary.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace Fit
        {
            class ReplicaVariable : public Variable
            {
            public:
                virtual ~ReplicaVariable() noexcept;

                template <typename ID> inline
                explicit ReplicaVariable(const ID &id, const Variable::Handle &hv) :
                Variable(id),
                primary(hv)
                {
                }
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(ReplicaVariable);
                virtual size_t idx()      const noexcept; //!< primary index
                virtual void   dsp(std::ostream &) const; //!< display primary info

                const Variable::Handle primary;
            };

        }

    }

}

#endif

