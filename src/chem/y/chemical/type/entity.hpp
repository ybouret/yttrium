
//! \file

#ifndef Y_Chemical_Entity_Included
#define Y_Chemical_Entity_Included 1

#include "y/graphviz/vizible.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! indexing level
        //
        //
        //______________________________________________________________________
        enum Level
        {
            TopLevel = 0, //!< in Library
            SubLevel = 1, //!< in Cluster
            AuxLevel = 2, //!< in a group
        };

        //______________________________________________________________________
        //
        //
        //
        //! named entity
        //
        //
        //______________________________________________________________________
        class Entity :
        public Object,
        public Counted,
        public GraphViz::Vizible
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const size_t LEVELS = AuxLevel+1; //!< number of levels

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! initialize with name and top-level index
            template <typename NAME> inline
            explicit Entity(const NAME & _name,
                            const size_t _indx) :
            Object(),
            Counted(),
            GraphViz::Vizible(),
            name(_name),
            indx()
            {
                assert(_indx>0);
                setup(_indx);
            }

            virtual ~Entity() noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Entity);    //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const String & key() const noexcept; //!< name

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const String name;         //!< name
            const size_t indx[LEVELS]; //!< indices

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Entity);
            void setup(const size_t _indx) noexcept;

        };

    }
}

#endif
