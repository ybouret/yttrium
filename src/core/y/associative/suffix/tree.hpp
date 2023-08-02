//! \file


#ifndef Y_Associative_Suffix_Tree_Included
#define Y_Associative_Suffix_Tree_Included 1

#include "y/associative/suffix/pool.hpp"
#include "y/associative/suffix/core/tree.hpp"
#include "y/data/list.hpp"

namespace Yttrium
{
    template <class KEY, class T, class NODE>
    class SuffixTree
    {
    public:
        Y_ARGS_DECL(T,Type);
        Y_ARGS_DECL(KEY,Key);
        typedef NODE                   NodeType;
        typedef ListOf<NODE>           ListType;
        typedef SuffixPool<NODE>       PoolType;
        typedef Memory::ReadOnlyBuffer PathType;

        explicit SuffixTree() : list(), pool(), tree() {}
        virtual ~SuffixTree() noexcept { hardReset(); }

        bool insert(ParamKey key, ParamType t)
        {
            return insert_( pool.template create<KEY,T>(key,t) );
        }

        bool insert(ParamType t)
        {
            return insert_( pool.template create<T>(t) );
        }


    protected:
        ListType         list;
        PoolType         pool;
        Core::SuffixTree tree;

        inline void hardReset() noexcept {
            tree.free();
            while(list.size>0)
                pool.vaporize(list.popTail());
        }

        inline void softReset() noexcept {
            tree.free();
            while(list.size>0)
                pool.destruct(list.popTail());
        }

    private:
        Y_DISABLE_ASSIGN(SuffixTree);



        inline bool insert_(NODE *node)
        {
            assert(0!=node);
            const PathType &path = node->key;
            try {
                if( 0 == (node->knot = tree.insert(path,node)) )
                {
                    pool.destruct(node);
                    return false;
                }
                list.pushTail(node);
                return true;
            }
            catch(...) { pool.destruct(node); throw; }
        }

    };
}

#endif

