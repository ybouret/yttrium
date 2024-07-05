
//! \file

#ifndef Y_Chemical_Indexed_Included
#define Y_Chemical_Indexed_Included 1

#include "y/chemical/entity.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //! index level
        //
        //______________________________________________________________________
        enum Level
        {
            TopLevel, //!< top     index
            SubLevel, //!< sub     index
            AuxLevel  //!< sub-sub index
        };


        //______________________________________________________________________
        //
        //
        //
        //! Entity with indices
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
            static const size_t Levels = AuxLevel; //!< number of indices

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup with identifier and top level index
            template <typename ID> inline
            explicit Indexed(const ID &   userName,
                             const size_t topLevel) :
            Entity(userName), indx()
            {
                setAllIndices(topLevel);
            }
            virtual ~Indexed() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t indx[Levels];  //!< indices

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Indexed);
            void setAllIndices(const size_t level);

        };
    }
}

#endif
