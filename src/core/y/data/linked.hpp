//! \file

#ifndef Y_Data_Linked_Included
#define Y_Data_Linked_Included 1

#include "y/config/starting.hpp"
#include <iostream>

namespace Yttrium
{

    namespace Core
    {
        //______________________________________________________________________
        //
        //
        //
        //! base class/metrics for Linked classes
        //
        //
        //______________________________________________________________________
        class Linked
        {
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
        protected:
            explicit Linked() noexcept; //!< setup
        public:
            virtual ~Linked() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual void reverse() noexcept = 0; //!< reverse order


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t size; //!< number of nodes

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Linked);
        };

    }


    //__________________________________________________________________________
    //
    //
    //
    //! Generic Linked class
    //
    //
    //__________________________________________________________________________
    template <typename NODE>
    class Linked : public Core::Linked
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        typedef NODE NodeType; //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        inline explicit Linked() noexcept : Core::Linked(), head(0) {} //!< setup empty
        inline virtual ~Linked() noexcept { assert(0==head); }         //!< cleanup


        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        //! check ownership
        inline bool owns(const NODE *node) const noexcept
        {
            const NODE *scan = head;
            for(size_t i=size;i>0;--i,scan=scan->next)
            {
                assert(0!=scan);
                if(scan == node) return true;
            }
            return false;
        }

        //! display, relying on a NODE::operator*()
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

        //______________________________________________________________________
        //
        //
        // Member
        //
        //______________________________________________________________________
        NODE *head; //!< head node for List/Pool

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Linked);
    };




}

#endif

