
//! \file
#ifndef Y_Jive_Group_Included
#define Y_Jive_Group_Included 1


#include "y/jive/entity.hpp"

namespace Yttrium
{
    namespace Jive
    {

        //______________________________________________________________________
        //
        //
        //
        //! Group to help format Entity based output
        //
        //
        //______________________________________________________________________
        class Group
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Group() noexcept; //!< setup
            virtual ~Group() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! update maxNameSize
            void     updateWith(const Entity &) noexcept;

            //! pad according to entity
            template <typename OSTREAM> inline
            OSTREAM & pad(OSTREAM &os, const Entity &ent) const
            {
                const size_t n=ent.name->size();
                for(size_t i=n;i<maxNameSize;++i) os << ' ';
                return os;
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t maxNameSize; //!< max name size for entities



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Group);
        };

    }

}

#endif
