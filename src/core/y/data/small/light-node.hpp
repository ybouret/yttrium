

//! \file

#ifndef Y_Data_Small_Light_Node_Included
#define Y_Data_Small_Light_Node_Included 1

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
        //! Node holding a reference to an existing object
        //
        //
        //______________________________________________________________________
        template <typename T>
        class LightNode
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            Y_ARGS_EXPOSE(T,Type);     //!< aliases
            typedef Type & ParamType;  //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline  LightNode(ParamType args) noexcept: next(0), prev(0), data(args) { } //!< no throw setup
            inline ~LightNode() noexcept { assert(0==next); assert(0==prev); }           //!< cleanup

            //! no-throw copy
            inline  LightNode(const LightNode &other) noexcept :
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
            inline friend std::ostream & operator<<(std::ostream &os, const LightNode &node)
            { return os << node.data; }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            LightNode *next; //!< for list/pool
            LightNode *prev; //!< for list
            
        private:
            Type     &data;
            Y_DISABLE_ASSIGN(LightNode);
        };
    }
}

#endif
