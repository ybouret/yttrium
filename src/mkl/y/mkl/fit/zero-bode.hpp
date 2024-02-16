
#ifndef Y_Fit_Zero_Bode_Included
#define Y_Fit_Zero_Bode_Included 1

#include "y/mkl/fit/memory-model.hpp"
#include "y/mkl/algebra/lu.hpp"
#include "y/sequence/snake.hpp"
#include "y/oversized.hpp"
#include "y/string/fwd.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace Fit
        {
            class ZeroBodeInfo : public Oversized
            {
            public:
                const size_t size;

                virtual ~ZeroBodeInfo() noexcept;
            protected:
                explicit ZeroBodeInfo(const size_t n) noexcept;

                void singular() const;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(ZeroBodeInfo);
            };

            template <typename ABSCISSA>
            class ZeroBode : public ZeroBodeInfo
            {
            public:
                typedef Snake<ABSCISSA> Scope;
                
                explicit ZeroBode(const size_t depth);
                virtual ~ZeroBode() noexcept;

                void free() noexcept;
                void feed(const ABSCISSA xx, const ABSCISSA yy);
                void save(const Core::String<char> &fileName) const;

                ABSCISSA inferred();

                const Scope  x;
                const Scope  y;
                LU<ABSCISSA> lu;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(ZeroBode);
            };
        }

    }

}

#endif

