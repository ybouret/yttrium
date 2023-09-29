//! \file


#ifndef Y_Chemical_Species_Included
#define Y_Chemical_Species_Included 1

#include "y/chem/type/entity.hpp"
#include "y/ptr/ark.hpp"

namespace Yttrium
{
    namespace Chemical
    {




        //______________________________________________________________________
        //
        //
        //
        //! Base class for species
        //
        //
        //______________________________________________________________________
        class Species : public Entity
        {
        public:
            typedef ArkPtr<String,Species> Pointer;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup from name + algebraic charge
            template <typename NAME> inline
            explicit Species(const NAME &uid,
                             const int   algebraicCharge) :
            Entity(uid), z(algebraicCharge) {}
            virtual ~Species() noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Species);

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            String toString() const; //!< "[name]"


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const int z; //!< algebraic charge

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Species);
        };
    }
}

#endif

