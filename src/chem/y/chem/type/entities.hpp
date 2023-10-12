//! \file


#ifndef Y_Chemical_Entities_Included
#define Y_Chemical_Entities_Included 1

#include "y/chem/type/entity.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! metrics to display entities
        //
        //
        //______________________________________________________________________
        class Entities
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Entities()        noexcept; //!< setup
            virtual ~Entities()        noexcept; //!< cleanup
            Entities(const Entities &) noexcept; //!< copy
            
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! record name size
            void updateWith(const Entity &) noexcept;

            //! record with size
            void updateWith(const size_t) noexcept;


            //! pad stream
            template <typename OSTREAM> inline
            OSTREAM & pad(OSTREAM &os, const size_t n) const
            {
                for(size_t i=n;i<maxNameSize;++i) os << ' ';
                return os;
            }

            //! pad stream
            template <typename OSTREAM> inline
            OSTREAM & pad(OSTREAM &os, const Entity &ent) const
            {
                return pad(os,ent.name.size());
            }


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t maxNameSize; //!< max name size of entities

        private:
            Y_DISABLE_ASSIGN(Entities);
        };

    }

}

#endif

