
//! \file

#ifndef Y_Associative_Included
#define Y_Associative_Included 1

#include "y/container/dynamic.hpp"
#include "y/type/args.hpp"
#include "y/data/list.hpp"
#include "y/memory/blanks.hpp"

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

    typedef ListOf<HashTableKnot> HashTableSlot;
    typedef Blanks<HashTableKnot> HashTablePool;

    class HashTable
    {
    public:
        static const unsigned LoadFactorLn2 = 2;
        static const size_t   LoadFactor    = 1 << LoadFactorLn2;

        explicit HashTable(const size_t sz);
        virtual ~HashTable() noexcept;

        HashTableSlot &       operator[](const size_t hkey) noexcept;
        const HashTableSlot & operator[](const size_t hkey) const noexcept;

        void    grow();
        size_t  size() const noexcept;
        void    freeWith(HashTablePool    &) noexcept;
        void    releaseWith(HashTablePool &) noexcept;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(HashTable);
        class Code;
        Code *code;
    };


}

#endif
