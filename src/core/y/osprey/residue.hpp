
//! \file


#ifndef Y_Osprey_Residue_Included
#define Y_Osprey_Residue_Included 1


#include "y/osprey/iproxy.hpp"

namespace Yttrium
{
    namespace Osprey
    {
        class Residue : public IProxy
        {
        public:

            explicit Residue(const IBank &bank,
                             const size_t dims,
                             const size_t excl);

            explicit Residue(const IBank &       bank,
                             const INode * const node);

            virtual ~Residue() noexcept;

            //! remove index of null vector
            void removeNull(const size_t zid) noexcept;





        private:
            Y_DISABLE_COPY_AND_ASSIGN(Residue);
            friend class Content;
        };
    }

}

#endif

