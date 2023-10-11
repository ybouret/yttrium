
//! \file

#ifndef Y_Chemical_Equilibria_Included
#define Y_Chemical_Equilibria_Included 1

#include "y/chem/reactive/equilibrium.hpp"
#include "y/chem/type/entities.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //! database of equilibria
        //
        //______________________________________________________________________
        typedef SuffixSet<String,Equilibrium::Pointer> EqDB;

        //______________________________________________________________________
        //
        //
        //
        //! Equilibria is a database of equilibria
        //
        //
        //______________________________________________________________________
        class Equilibria : public Entities, public Proxy<const EqDB>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< Equilibria

            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            explicit Equilibria();           //!< setup empty
            virtual ~Equilibria() noexcept;  //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! insert a newly create equiliibriuam and initial indices
            template <typename EQ> inline
            EQ & operator()(EQ *eq)
            {
                assert(0!=eq);
                submit(eq);
                return *eq;
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibria);
            virtual ConstInterface & surrogate() const noexcept;
            void                     submit(Equilibrium *eq);
            EqDB db;
        };

    }

}

#endif
