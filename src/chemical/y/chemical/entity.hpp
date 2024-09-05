
//! \file

#ifndef Y_Chemical_Entity_Included
#define Y_Chemical_Entity_Included 1

#include "y/graphviz/vizible.hpp"
#include "y/string.hpp"
#include "y/quantized.hpp"
#include "y/associative/little-endian-addressed.hpp"

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
        class Entity : 
        public Quantized,
        public Counted,
        public LittleEndianAddressed,
        public GraphViz::Vizible
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
            Quantized(),
            Counted(),
            LittleEndianAddressed(),
            GraphViz::Vizible(),
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

