//! \file

#ifndef Y_Data_Small_Heavy_Node_Included
#define Y_Data_Small_Heavy_Node_Included 1

#include "y/type/args.hpp"

namespace Yttrium
{
    namespace Small
    {
        //! node with full inside object
        template <typename T>
        class HeavyNode
        {
        public:
            Y_ARGS_EXPOSE(T,Type);

            inline  HeavyNode(ConstType &args) : next(0), prev(0), data(args) { }
            inline ~HeavyNode() noexcept { assert(0==next); assert(0==prev); }
            inline  HeavyNode(const HeavyNode &other) :
            next(0),
            prev(0),
            data(other.data)
            {
            }

            inline Type      & operator*()       noexcept { return data; }
            inline ConstType & operator*() const noexcept { return data; }


            HeavyNode *next;
            HeavyNode *prev;
        private:
            Type      data;
            Y_DISABLE_ASSIGN(HeavyNode);
        };
    }
}

#endif
