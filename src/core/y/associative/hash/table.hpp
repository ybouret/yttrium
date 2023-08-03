
//! \file

#ifndef Y_Associative_Included
#define Y_Associative_Included 1

#include "y/container/dynamic.hpp"
#include "y/type/args.hpp"

namespace Yttrium
{

   
    class HashTable
    {
    public:
        explicit HashTable(const size_t);
        virtual ~HashTable() noexcept;
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(HashTable);
        class Code;
        Code *code;
    };


}

#endif
