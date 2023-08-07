//! \file

#ifndef Y_Associative_Hash_Table_Included
#define Y_Associative_Hash_Table_Included 1

#include "y/container/dynamic.hpp"
#include "y/type/args.hpp"
#include "y/data/list.hpp"
#include "y/memory/blanks.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Anonymous knot to point to data within table
    //
    //
    //__________________________________________________________________________
    class HashKnot
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        typedef ListOf<HashKnot> List; //!< internal list
        typedef Blanks<HashKnot> Pool; //!< memory management

        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        HashKnot(const size_t,void *) noexcept; //!< setup with hkey and user's node
        ~HashKnot()                   noexcept; //!< cleanup

        //______________________________________________________________________
        //
        //
        // Members
        //
        //______________________________________________________________________
        HashKnot      *next; //!< for list
        HashKnot      *prev; //!< for list
        const size_t   hkey; //!< hash key
        void * const   node; //!< user's node



    private:
        Y_DISABLE_COPY_AND_ASSIGN(HashKnot);
    };


    //__________________________________________________________________________
    //
    //
    //
    //! Table of Knots for hash-based lookup
    //
    //
    //__________________________________________________________________________
    class HashTable
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        static const unsigned LoadFactorLn2 = 2;                   //!< log2(LoadFactor)
        static const size_t   LoadFactor    = 1 << LoadFactorLn2;  //!< LoadFactor

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        explicit HashTable(const size_t sz); //!< table with minimal size
        virtual ~HashTable() noexcept;       //!< cleanup (must be empty)

        //______________________________________________________________________
        //
        //
        // Members
        //
        //______________________________________________________________________
        HashKnot::List &       operator[](const size_t hkey) noexcept;        //!< internal slot for hkey
        const HashKnot::List & operator[](const size_t hkey) const noexcept;  //!< internal slot for hkey

        void    freeWith(HashKnot::Pool    &) noexcept; //!< free all knots, keep memory
        void    releaseWith(HashKnot::Pool &) noexcept; //!< free all knots, release memory
        size_t  size()                  const noexcept; //!< current size
        void    grow();                                 //!< double size

    private:
        Y_DISABLE_COPY_AND_ASSIGN(HashTable);
        class Code;
        Code *code;
    };


}

#endif
