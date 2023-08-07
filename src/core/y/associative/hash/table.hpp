//! \file

#ifndef Y_Associative_Hash_Table_Included
#define Y_Associative_Hash_Table_Included 1

#include "y/container/dynamic.hpp"
#include "y/type/args.hpp"
#include "y/data/list.hpp"
#include "y/memory/blanks.hpp"

namespace Yttrium
{

    //! anonymous knot
    class HashKnot
    {
    public:
        HashKnot(const size_t,void *) noexcept;
        ~HashKnot()                   noexcept;

        HashKnot      *next;
        HashKnot      *prev;
        const size_t   hkey;
        void * const   node;

        typedef ListOf<HashKnot> List;
        typedef Blanks<HashKnot> Pool;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(HashKnot);
    };


    class HashTable
    {
    public:
        static const unsigned LoadFactorLn2 = 2;
        static const size_t   LoadFactor    = 1 << LoadFactorLn2;

        explicit HashTable(const size_t sz);
        virtual ~HashTable() noexcept;

        HashKnot::List &       operator[](const size_t hkey) noexcept;
        const HashKnot::List & operator[](const size_t hkey) const noexcept;

        void    grow();
        size_t  size() const noexcept;
        void    freeWith(HashKnot::Pool    &) noexcept;
        void    releaseWith(HashKnot::Pool &) noexcept;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(HashTable);
        class Code;
        Code *code;
    };


}

#endif
