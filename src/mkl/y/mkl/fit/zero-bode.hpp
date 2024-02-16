
#ifndef Y_Fit_Zero_Bode_Included
#define Y_Fit_Zero_Bode_Included 1

#include "y/mkl/fit/memory-model.hpp"
#include "y/mkl/algebra/lu.hpp"
#include "y/sequence/snake.hpp"
#include "y/oversized.hpp"
#include "y/string.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace Fit
        {
            class ZeroBodeInfo : public Oversized
            {
            public:

                virtual ~ZeroBodeInfo() noexcept;
            protected:
                explicit ZeroBodeInfo() noexcept;

                void singular() const;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(ZeroBodeInfo);
            };

            template <typename ABSCISSA>
            class ZeroBode : public ZeroBodeInfo, public Snake<ABSCISSA>
            {
            public:
                typedef Snake<ABSCISSA> Scope;
                
                explicit ZeroBode(const size_t depth);
                virtual ~ZeroBode() noexcept;

                void save(const String &fileName) const;

                ABSCISSA inferred();
                LU<ABSCISSA> lu;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(ZeroBode);
            };
        }

    }

}

#endif

