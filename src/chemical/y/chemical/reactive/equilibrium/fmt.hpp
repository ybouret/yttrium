//! \file

#ifndef Y_Chemical_Equilibrium_Fmt_Included
#define Y_Chemical_Equilibrium_Fmt_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/chemical/type/entities.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Helper to format equilibria/cluster
        //
        //
        //______________________________________________________________________
        class EqFormatter : public Entities, public Components::Formatting
        {
        public:
            explicit EqFormatter() noexcept; //!< setup
            virtual ~EqFormatter() noexcept; //!< cleanup
            void           upgradeWith(const Equilibrium &)          noexcept; //!< update/modernize
            std::ostream & display(std::ostream &, const Equilibrium &) const; //!< display

        private:
            Y_DISABLE_COPY_AND_ASSIGN(EqFormatter);
        };
    }

}

#endif

