
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
            static const char * const   CallSign;      //!< Equilibria
            typedef EqDB::ConstIterator ConstIterator; //!< alias
            typedef EqDB::Iterator      Iterator;      //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Equilibria();           //!< setup empty
            virtual ~Equilibria() noexcept;  //!< cleanup
            Y_OSTREAM_PROTO(Equilibria);     //!< pretty display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! insert a newly created equilibrium and initial indices
            template <typename EQ> inline
            EQ & operator()(EQ *eq)
            {
                assert(0!=eq);
                submit(eq);
                return *eq;
            }

            //! update maxReac and maxProd
            void updateMaxSizes() noexcept;

            Iterator begin() noexcept { return db.begin(); }
            Iterator end()   noexcept { return db.end();   }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibria);
            virtual ConstInterface & surrogate() const noexcept;
            void                     submit(Equilibrium *eq);
            EqDB db;

        public:
            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Entities maxReac; //!< helper to display
            const Entities maxProd; //!< helper to display
        };

    }

}

#endif
