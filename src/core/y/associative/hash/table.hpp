
//! \file

#ifndef Y_Associative_Included
#define Y_Associative_Included 1

#include "y/container/dynamic.hpp"
#include "y/type/args.hpp"
#include "y/data/list.hpp"

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


    class HashTable
    {
    public:
        explicit HashTable(const size_t);
        virtual ~HashTable() noexcept;

        HashTableSlot &       operator[](const size_t hkey) noexcept;
        const HashTableSlot & operator[](const size_t hkey) const noexcept;

        void    grow();
        size_t  size() const noexcept;
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(HashTable);
        class Code;
        Code *code;
    };


}

#endif
