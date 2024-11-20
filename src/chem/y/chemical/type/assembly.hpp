
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
        //! helper to format assemblies of entites
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
            explicit Assembly()        noexcept; //!< setup
            virtual ~Assembly()        noexcept; //!< cleanup
            Assembly(const Assembly &) noexcept; //!< copy

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void enroll(const Entity &) noexcept;

            //! os << justified((*curr).name); proc(os,*curr)
            template <typename ITERATOR, typename PROC> inline
            void forEach(ITERATOR            curr,
                         size_t              size,
                         PROC &              proc,
                         std::ostream &      os,
                         const Justify::Type just)
            {
                while(size-- > 0)
                {
                    os << Justify((*curr).name,maxNameSize,just);
                    proc(os,*curr);
                    os << std::endl;
                    ++curr;
                }
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t maxNameSize; //!< or enrolled species
        private:
            Y_DISABLE_ASSIGN(Assembly);

        };
    }

}

#endif

