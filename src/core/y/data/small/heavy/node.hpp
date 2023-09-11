//! \file

#ifndef Y_Data_Small_Heavy_Node_Included
#define Y_Data_Small_Heavy_Node_Included 1

#include "y/type/args.hpp"
#include <iostream>

namespace Yttrium
{
    namespace Small
    {
        //______________________________________________________________________
        //
        //
        //
        //! Node holding some data
        //
        //
        //______________________________________________________________________
        template <typename T>
        class HeavyNode
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            Y_ARGS_EXPOSE(T,Type);         //!< aliases
            typedef ConstType & ParamType; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup with data
            inline  HeavyNode(ParamType args) :
            next(0),
            prev(0),
            data(args)
            {   }

            inline ~HeavyNode() noexcept { assert(0==next); assert(0==prev); }   //!< cleanup

            //! copy data content
            inline  HeavyNode(const HeavyNode &other) :
            next(0),
            prev(0),
            data(other.data)
            {
            }

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            inline Type      & operator*()       noexcept { return data; } //!< access
            inline ConstType & operator*() const noexcept { return data; } //!< access, const

            //! forward display to data
            inline friend std::ostream & operator<<(std::ostream &os, const HeavyNode &node)
            { return os << node.data; }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            HeavyNode *next; //!< for list/pool
            HeavyNode *prev; //!< for list

        private:
            Type      data;
            Y_DISABLE_ASSIGN(HeavyNode);
        };
    }
}

#endif
