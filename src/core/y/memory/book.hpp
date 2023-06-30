
//! \file

#ifndef Y_Memory_Book_Included
#define Y_Memory_Book_Included 1

#include "y/memory/pages.hpp"
#include "y/calculus/align.hpp"

namespace Yttrium
{
    
    namespace Memory
    {
        class Book
        {
        public:
            static const size_t Required = Pages::Number * sizeof(Pages);
            explicit Book();
            virtual ~Book() noexcept;

        private:
            Pages *pages;
            void  *pages_[Y_WORDS_GEQ(Required)];
        };
    }

}

#endif

