
//! \file

#ifndef Y_Associative_Included
#define Y_Associative_Included 1

#include "y/container/dynamic.hpp"
#include "y/type/args.hpp"

namespace Yttrium
{

    class HashTableKnot
    {
    public:
        HashTableKnot(const size_t,void *) noexcept;
        ~HashTableKnot()                   noexcept;

        HashTableKnot *next;
        HashTableKnot *prev;
        const size_t   hkey;
        void * const   node;
    private:
        Y_DISABLE_COPY_AND_ASSIGN(HashTableKnot);
    };

    class HashTable
    {
    public:
        explicit HashTable(const size_t);
        virtual ~HashTable() noexcept;

        void   insert(HashTableKnot *knot) noexcept;

        size_t  size() const noexcept;
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(HashTable);
        class Code;
        Code *code;
    };


}

#endif
