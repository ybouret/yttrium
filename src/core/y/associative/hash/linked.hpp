
//! \file

#ifndef Y_Associative_Hash_Linked_Included
#define Y_Associative_Hash_Linked_Included 1

#include "y/associative/hash/table.hpp"

namespace Yttrium
{

    template <
    typename KEY,
    typename T,
    typename NODE,
    typename KEY_HASHER>
    class HashLinked
    {
    public:
        typedef Blanks<NODE> NodePool;

        explicit HashLinked() : nodes(), table(0), kpool(0), npool(0) {}

    protected:
        ListOf<NODE>  nodes; //!< actual data
        HashTable     table; //!< current table
        HashTablePool kpool; //!< pool of knots
        NodePool      npool; //!< pool of nodes

    private:
        Y_DISABLE_COPY_AND_ASSIGN(HashLinked);
    };

}

#endif

