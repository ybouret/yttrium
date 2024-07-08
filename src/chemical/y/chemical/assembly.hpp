
//! \file

#ifndef Y_Chemical_Assembly_Included
#define Y_Chemical_Assembly_Included 1

#include "y/chemical/entity.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Assembly to process output of entity
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
            explicit Assembly() noexcept;
            virtual ~Assembly() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! update with entity
            void enroll(const Entity &) noexcept;

            //! left justify
            template <typename OSTREAM> inline
            OSTREAM & lj(OSTREAM      &os,
                         const String &st) const
            {
                os << st;
                for(size_t i=st.size();i<maxLen;++i) os << ' ';
                return os;
            }

            //! right justify
            template <typename OSTREAM> inline
            OSTREAM & rj(OSTREAM      &os,
                         const String &st) const
            {
                for(size_t i=st.size();i<maxLen;++i) os << ' ';
                os << st;
                return os;
            }

            //__________________________________________________________________
            //
            //
            // Mmembers
            //
            //__________________________________________________________________
            const size_t maxLen; //!< max length of entities name
                                 
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Assembly);
        };
    }

}

#endif

