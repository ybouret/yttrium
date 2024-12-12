
//! \file

#ifndef Y_Chemical_Mixes_Included
#define Y_Chemical_Mixes_Included 1

#include "y/chemical/plexus/mix.hpp"
#include "y/chemical/reactive/equilibria.hpp"
#include "y/chemical/plexus/conservation/laws.hpp"

#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! list of indepedent mixes of dependents equilibria
        //
        //
        //______________________________________________________________________
        class Mixes : public Proxy<const Mix::List>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "Chemical::Mixes"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup/update from equilibria
            explicit Mixes(Equilibria &eqs,
                           XMLog  &    xml);
            virtual ~Mixes() noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Mixes);    //!< display

        private:
            Mix::List my;

            Y_DISABLE_COPY_AND_ASSIGN(Mixes);
            virtual ConstInterface & surrogate() const noexcept;
            void                     checkFusion();

        };

    }

}

#endif

