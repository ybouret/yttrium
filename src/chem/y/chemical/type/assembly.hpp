
//! \file

#ifndef Y_Chemical_Assembly_Included
#define Y_Chemical_Assembly_Included 1

#include "y/chemical/type/entity.hpp"
#include "y/text/justify.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Helper to format assembly of entities
        //
        //
        //______________________________________________________________________
        class Assembly
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Assembly()         noexcept; //!< setup
            virtual ~Assembly()         noexcept; //!< cleanup
            Assembly(const Assembly &_) noexcept; //!< copy

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void enroll(const Entity &) noexcept; //!< check maxKeySize

            //! recompute upon deletion/error
            template <typename LIST> inline
            void recompute(const LIST &list) noexcept
            {
                Coerce(maxKeySize) = 0;
                for(const typename LIST::NodeType *node=list.head;node;node=node->next)
                {
                    const Entity &entity = *node;
                    enroll( entity );
                }

            }

            //! helper to format an entity key among this assembly
            template <typename OSTREAM> inline
            OSTREAM & print(OSTREAM            &os,
                            const char * const  pfx,
                            const Entity       &entity,
                            const char * const  sfx,
                            const Justify::Type jtype) const
            {
                const String  lhs = pfx;
                const String  rhs = sfx;
                const String  out = lhs + entity.key() + rhs;
                const Justify jst(out,maxKeySize+lhs.size()+rhs.size(),jtype);
                return os << jst;
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t maxKeySize; //!< max of entities key size
        private:
            Y_DISABLE_ASSIGN(Assembly);
        };

    }

}

#endif
