//! \file


#ifndef Y_Chemical_Entity_Included
#define Y_Chemical_Entity_Included 1

#include "y/graphviz/vizible.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //! indexation level
        enum Level
        {
            TopLevel = 0, //!< top level index, within library/equilibria
            SubLevel = 1, //!< sub level index, within cluster
            AuxLevel = 2, //!< aux level index, within conservations
            Levels        //!< number of levels
        };

        //______________________________________________________________________
        //
        //
        //
        //! Named entity interface
        //
        //
        //______________________________________________________________________
        class Entity : public Object, public Counted, public Vizible
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup from name
            template <typename NAME>
            explicit inline Entity(const NAME &uid) :
            Object(), 
            Counted(),
            name(uid),
            indx()
            {
                zindx();
            }

            //! copy
            Entity(const Entity &);

            //! cleanup
            virtual ~Entity() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const String &key() const noexcept; //!< key for databases

            //__________________________________________________________________
            //
            //
            // Member
            //
            //__________________________________________________________________s
            const String  name;          //!< identifier
            const size_t  indx[Levels];  //!< indices
            
        private:
            Y_DISABLE_ASSIGN(Entity);
            void zindx() noexcept;
        };
    }
}

#endif

