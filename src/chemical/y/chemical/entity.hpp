
//! \file

#ifndef Y_Chemical_Entity_Included
#define Y_Chemical_Entity_Included 1

#include "y/graphviz/vizible.hpp"
#include "y/string.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! base class for entities
        //
        //
        //______________________________________________________________________
        class Entity : public Object, public Counted, public GraphViz::Vizible
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
          
            //! setup with any name
            template <typename ID> inline
            explicit Entity(const ID & userName ) :
            name(userName)
            {
            }

            //! cleanup
            virtual ~Entity() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const String & key() const noexcept;     //!< for sets/maps

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const String name; //!< uuid
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Entity);
        };

    }

}

#endif

