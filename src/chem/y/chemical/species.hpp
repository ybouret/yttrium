
//! \file

#ifndef Y_Chemical_Species_Included
#define Y_Chemical_Species_Included 1

#include "y/chemical/type/entity.hpp"
#include "y/ptr/ark.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Species is a charged entity
        //
        //
        //______________________________________________________________________
        class Species : public Entity
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef ArkPtr<const String,const Species> Handle; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup with name and top-level index
            template <typename NAME> inline
            explicit Species(const NAME & _name,
                             const int    _z,
                             const size_t _indx) :
            Entity(_name,_indx),
            z(_z)
            {
            }

            //! cleanup
            virtual ~Species() noexcept;

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

