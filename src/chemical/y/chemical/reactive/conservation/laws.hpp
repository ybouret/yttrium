
//! \file

#ifndef Y_Chemical_Conservation_Laws_Included
#define Y_Chemical_Conservation_Laws_Included 1

#include "y/chemical/reactive/conservation/law.hpp"
#include "y/container/matrix.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {

            class Laws : public Proxy<const Law::List>
            {
            public:
                explicit Laws() noexcept;
                virtual ~Laws() noexcept;

                void writeDown(const Matrix<unsigned> &Qm,
                               const SpSubSet         &table);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Laws);
                Law::List cll;

                virtual ConstInterface & surrogate() const noexcept;
            };
        }

    }

}

#endif
