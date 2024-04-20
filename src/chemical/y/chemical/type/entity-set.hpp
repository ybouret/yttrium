//! \file

#ifndef Y_Chemical_Entity_Set_Included
#define Y_Chemical_Entity_Set_Included 1

#include "y/chemical/type/entity.hpp"
#include "y/associative/hash/set.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        template <typename ENTITY, Level LEVEL>
        class EntityRef
        {
        public:
            inline  EntityRef(const ENTITY &usr) noexcept : entity(usr) {}
            inline ~EntityRef() noexcept {}
            inline  EntityRef(const EntityRef &ref) noexcept : entity(ref.entity) {}

            inline const ENTITY & operator*() const noexcept { return entity; }

            inline const size_t & key() const noexcept { return entity.indx[LEVEL]; }

        private:
            Y_DISABLE_ASSIGN(EntityRef);
            const ENTITY &entity;
        };

        template <typename ENTITY, Level LEVEL>
        class EntitySet : public HashSet<size_t,EntityRef<ENTITY,LEVEL> >
        {
        public:
            typedef EntityRef<ENTITY,LEVEL>    RefType;
            typedef HashSet<size_t,RefType>    SetType;
            inline explicit EntitySet() : SetType() {}
            inline virtual ~EntitySet() noexcept {}

            inline bool record(const ENTITY &args) {
                const RefType ref(args);
                return this->insert(ref);
            }

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

