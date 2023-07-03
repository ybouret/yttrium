
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
            static const size_t       Required = Pages::Number * sizeof(Pages);
            static const char * const CallSign;
            
            explicit Album();
            virtual ~Album() noexcept;

            Pages & operator[](const unsigned shift) noexcept;
            void displayInfo(const size_t indent) const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Album);
            Pages *pages;
            void  *pages_[Y_WORDS_GEQ(Required)];
        };
    }

}

#endif

