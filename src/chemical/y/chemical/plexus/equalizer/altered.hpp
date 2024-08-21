
//! \file

#ifndef Y_Chemical_Altered_Included
#define Y_Chemical_Altered_Included 1

#include "y/chemical/reactive/equilibrium.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! altered balanced state for equalizer
        //
        //
        //______________________________________________________________________
        class Altered
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef CxxSeries<Altered,XMemory> Series; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
           
            //! setup with persistent data
            explicit Altered(const Equilibrium & _eq,
                             const xreal_t       _xi,
                             const XReadable   & _cc,
                             const xreal_t       _gg,
                             const XReadable   & _dc) noexcept :
            eq(_eq),
            xi(_xi),
            cc(_cc),
            gg(_gg),
            dc(_dc)
            {
            }

            //! duplicate
            Altered(const Altered &_) noexcept :
            eq(_.eq),
            xi(_.xi),
            cc(_.cc),
            gg(_.gg),
            dc(_.dc)
            {
            }

            //! comparison by decreasing gain
            static int Compare(const Altered &lhs, const Altered &rhs) noexcept
            {
                return Comparison:: Decreasing(lhs.gg, rhs.gg);
            }

            //! cleanup
            ~Altered() noexcept {}

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Equilibrium & eq; //!< source equilibrium
            const xreal_t       xi; //!< extent to get cc for original
            const XReadable   & cc; //!< current state
            const xreal_t       gg; //!< current gain
            const XReadable   & dc; //!< dc from original concentration

        private:
            Y_DISABLE_ASSIGN(Altered);
        };
    }

}

#endif

