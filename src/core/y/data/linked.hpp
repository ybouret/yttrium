//! \file

#ifndef Y_Data_Linked_Included
#define Y_Data_Linked_Included 1

#include "y/config/starting.hpp"
#include <iostream>

namespace Yttrium
{

    namespace Core
    {
        class Linked
        {
        public:
            const size_t size;

            virtual ~Linked() noexcept;
        protected:
            explicit Linked() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Linked);
        };

    }

    template <typename NODE>
    class Linked : public Core::Linked
    {
    public:
        typedef NODE NodeType;
        
        inline explicit Linked() noexcept : Core::Linked(), head(0) {}
        inline virtual ~Linked() noexcept { assert(0==head); }

        inline friend std::ostream & operator<<(std::ostream &os, const Linked &self)
        {
            os << '[';
            if(self.size>0)
            {
                const NODE *node = self.head; assert(0!=node);
                os << **node;
                for(size_t i=self.size-1;i>0;--i)
                {
                    node = node->next; assert(0!=node);
                    os << ';' << **node;
                }
            }
            os << ']';
            return os;
        }

        NODE *head;


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Linked);
    };




}

#endif

