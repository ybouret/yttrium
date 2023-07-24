

//! \file

#ifndef Y_Data_Small_Repo_Node_Included
#define Y_Data_Small_Repo_Node_Included 1

#include "y/type/args.hpp"

namespace Yttrium
{
    namespace Small
    {
        template <typename T>
        class RepoNode
        {
        public:
            Y_ARGS_EXPOSE(T,Type);

            inline  RepoNode(ConstType &args) noexcept: next(0), prev(0), data(args) { }
            inline ~RepoNode() noexcept { assert(0==next); assert(0==prev); }
            inline  RepoNode(const RepoNode &other) noexcept :
            next(0),
            prev(0),
            data(other.data)
            {
            }

            inline Type      & operator*()       noexcept { return data; }
            inline ConstType & operator*() const noexcept { return data; }

            RepoNode *next;
            RepoNode *prev;
        private:
            Type     &data;
            Y_DISABLE_ASSIGN(RepoNode);
        };
    }
}

#endif
