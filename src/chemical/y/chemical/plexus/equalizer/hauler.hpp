
//! \file


#ifndef Y_Chemical_Hauler_Included
#define Y_Chemical_Hauler_Included 1

#include "y/chemical/reactive/components.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Equalizer
        {

            class Hauler : public SRepo
            {
            public:
                explicit Hauler(const SBank &sb) noexcept;
                explicit Hauler(const Hauler &);
                virtual ~Hauler() noexcept;

                virtual xreal_t getXi() const noexcept = 0;

                //! move best.xi, negative product cursor
                void            zforward(const Components &E,
                                         XWritable        &C,
                                         const Level       L) const noexcept;

                //! move negative reactant cursor
                void            zreverse(const Components &E,
                                         XWritable        &C,
                                         const Level       L) const noexcept;
            private:
                Y_DISABLE_ASSIGN(Hauler);
                void vanish(XWritable        &C,
                            const Level       L) const noexcept;
            };

        }

    }

}

#endif

