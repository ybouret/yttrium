//! \file

#ifndef Y_Chemical_Indexed_Included
#define Y_Chemical_Indexed_Included 1

#include "y/chemical/type/entity.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //! different levels of indexing
        //
        //______________________________________________________________________
        enum Level
        {
            TopLevel, //!< global index
            SubLevel, //!< index within cluster
            AuxLevel  //!< index within auxiliary  structures
        };

        //______________________________________________________________________
        //
        //
        //
        //! Indexed entity
        //
        //
        //______________________________________________________________________
        class Indexed : public Entity
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const size_t Levels = AuxLevel + 1; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            explicit Indexed(const size_t topIndex) noexcept; //!< topIndex>0
            virtual ~Indexed()                      noexcept; //!< cleanup


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t indx[Levels]; //!< relative indices

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Indexed);
        };
    }

}

#endif

