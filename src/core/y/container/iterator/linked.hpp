
//! \file

#ifndef Y_Iterator_Linked_Included
#define Y_Iterator_Linked_Included 1

#include "y/container/iterator/types.hpp"
#include "y/type/args.hpp"

namespace Yttrium
{

    namespace Iterating
    {
        //______________________________________________________________________
        //
        //
        //
        //! Linear iterator
        //
        //
        //______________________________________________________________________
        template <typename T, typename NODE, Direction WAY>
        class Linked : public Travel<WAY>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            Y_ARGS_EXPOSE(T,Type);    //!< aliases
            Y_ARGS_EXPOSE(NODE,Node); //!< aliases
            using Travel<WAY>::Way;

            inline Linked(NODE *user) noexcept : node( (MutableNode *)user ) {}
            inline virtual ~Linked() noexcept { node=0; }
            inline Linked(const Linked &other) noexcept : node(other.node) {}
            inline Linked & operator=(const Linked &other) noexcept
            {
                node = other.node;
                return *this;
            }

        private:
            MutableNode *node;
        };

    }

}
#endif

