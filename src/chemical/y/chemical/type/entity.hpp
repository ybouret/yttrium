//! \file

#ifndef Y_Chemical_Entity_Included
#define Y_Chemical_Entity_Included 1

#include "y/chemical/type/defs.hpp"
#include "y/graphviz/vizible.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //! level of indexing
        //
        //______________________________________________________________________
        enum Level
        {
            TopLevel = 0, //!< global index
            SubLevel = 1, //!< index in a  sub group
            AuxLevel = 2  //!< index in an auxiliary group
        };

        //______________________________________________________________________
        //
        //
        //
        //! Base class for entity with name=key() and indices
        //
        //
        //______________________________________________________________________
        class Entity : public Object, public Counted, public GraphViz::Vizible
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const size_t Levels = AuxLevel+1; //!< max number of levels

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup with name
            template <typename NAME> inline
            explicit Entity(const NAME &uid, const size_t topLevel) :
            Object(),
            Counted(),
            GraphViz::Vizible(),
            name(uid),
            indx()
            {
                initialize(topLevel);
            }

            //! cleanup
            virtual ~Entity() noexcept;
            
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const String & key() const noexcept; //!< name, for sets/maps

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const String name;          //!< unique identifier
            const size_t indx[Levels];  //!< indices

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Entity);
            void initialize(const size_t level) noexcept;
        };

    }
}

#endif

