//! \file

#ifndef Y_Chemical_Entity_Set_Included
#define Y_Chemical_Entity_Set_Included 1

#include "y/chemical/type/entity.hpp"
#include "y/associative/hash/set.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Lightweight wrapper to an entity reference
        //
        //
        //______________________________________________________________________
        template <typename ENTITY, Level LEVEL>
        class EntityRef
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline  EntityRef(const ENTITY &usr)    noexcept : entity(usr) {}        //!< setup from a persistent entity
            inline ~EntityRef()                     noexcept {}                      //!< cleanup
            inline  EntityRef(const EntityRef &ref) noexcept : entity(ref.entity) {} //!< copy

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            inline const ENTITY & operator*() const noexcept { return entity; }              //!< access
            inline const size_t & key()       const noexcept { return entity.indx[LEVEL]; }  //!< key for Set

        private:
            Y_DISABLE_ASSIGN(EntityRef);
            const ENTITY &entity;
        };

        //______________________________________________________________________
        //
        //
        //
        //! Lightweight Set of entities indexed by keys
        //
        //
        //______________________________________________________________________
        template <typename ENTITY, Level LEVEL>
        class EntitySet : public HashSet<size_t,EntityRef<ENTITY,LEVEL> >
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef EntityRef<ENTITY,LEVEL>    RefType; //!< alias
            typedef HashSet<size_t,RefType>    SetType; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline explicit EntitySet() : SetType() {} //!< setup
            inline virtual ~EntitySet() noexcept {}    //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
           
            //! record a persistent entity
            inline bool record(const ENTITY &args) {
                const RefType ref(args);
                return this->insert(ref);
            }

            //! access an EXISTING entity
            inline const ENTITY &operator[](const size_t indx) const noexcept
            {
                const RefType *pRef = this->search(indx); assert(0!=pRef);
                return **pRef;
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(EntitySet);
        };

    }

}

#endif

