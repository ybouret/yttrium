
//! \file

#ifndef Y_Chemical_Entity_Included
#define Y_Chemical_Entity_Included 1

#include "y/string.hpp"
#include "y/type/copy.hpp"

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
        //! Base class: name + indices
        //
        //
        //______________________________________________________________________
        class Entity : public Object, public Counted
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
            explicit Entity(const ID &   identifier,
                            const size_t topLevel) :
            name(identifier), indx()
            {
                setIndices(topLevel);
            }
            Entity(const CopyOf_ &, const Entity &); //!< full copy
            virtual ~Entity() noexcept;              //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const String & key() const noexcept; //!< for sets/maps

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const String name;          //!< uuid
            const size_t indx[Levels];  //!< indices

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Entity);
            void setIndices(const size_t level);
            
        };
    }
}

#endif
