
//! \file

#ifndef Y_Memory_Book_Included
#define Y_Memory_Book_Included 1

#include "y/memory/pages.hpp"
#include "y/calculus/align.hpp"

namespace Yttrium
{
    
    namespace Memory
    {
        class Album
        {
        public:
            static const size_t Required = Pages::Number * sizeof(Pages);
            explicit Album();
            virtual ~Album() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Album);
            Pages *pages;
            void  *pages_[Y_WORDS_GEQ(Required)];
        };
    }

}

#endif

