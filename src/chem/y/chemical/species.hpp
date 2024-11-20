
//! \file

#ifndef Y_Chemical_Species_Included
#define Y_Chemical_Species_Included 1

#include "y/chemical/type/entity.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Species : public Entity
        {
        public:
            template <typename NAME> inline
            explicit Species(const NAME & _name,
                             const int    _z,
                             const size_t _indx) :
            Entity(_name,_indx),
            z(_z)
            {
            }
            virtual ~Species() noexcept;
            
            const int z; //!< algebraic charge

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Species);
        };
    }

}

#endif

