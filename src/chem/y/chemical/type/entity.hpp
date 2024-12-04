
//! \file

#ifndef Y_Chemical_Entity_Included
#define Y_Chemical_Entity_Included 1

#include "y/chemical/type/lang.hpp"
#include "y/oversized.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! keyed entity
        //
        //
        //______________________________________________________________________
        class Entity : public Oversized
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Entity() noexcept; //!< setup
            virtual ~Entity() noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Entity);    //!< display name

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual const String & key() const noexcept = 0; //!< unique identifier

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Entity);
        };

       



    }

}


#endif
