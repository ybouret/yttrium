
//! \file


#ifndef Y_Osprey_Residue_Included
#define Y_Osprey_Residue_Included 1


#include "y/osprey/iproxy.hpp"

namespace Yttrium
{
    namespace Osprey
    {
        //______________________________________________________________________
        //
        //
        //
        //! Available row indices to build QFamily
        //
        //
        //______________________________________________________________________
        class Residue : public IProxy
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup with dimensions, but excluded index
            explicit Residue(const IBank &bank,
                             const size_t dims,
                             const size_t excl);

            //! setup with prev/next of node
            explicit Residue(const IBank &       bank,
                             const INode * const node);

            //! setup with single index
            explicit Residue(const IBank &bank,
                             const size_t indx);

            Residue(const Residue &); //!< duplicate

            //! cleanup
            virtual ~Residue() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! remove index of null vector
            void removeNull(const size_t zid) noexcept;


        private:
            Y_DISABLE_ASSIGN(Residue);
            friend class Content;
        };
    }

}

#endif

