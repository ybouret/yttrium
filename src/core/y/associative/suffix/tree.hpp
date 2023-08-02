//! \file


#ifndef Y_Associative_Suffix_Tree_Included
#define Y_Associative_Suffix_Tree_Included 1

#include "y/associative/suffix/pool.hpp"
#include "y/associative/suffix/core/tree.hpp"
#include "y/data/list.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! Generic SuffixTree for Map/Set
    //
    //
    //__________________________________________________________________________
    template <class KEY, class T, class NODE>
    class SuffixTree
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        Y_ARGS_DECL(T,Type);                     //!< aliases
        Y_ARGS_DECL(KEY,Key);                    //!< aliases
        typedef NODE                   NodeType; //!< aliases
        typedef ListOf<NODE>           ListType; //!< aliases
        typedef SuffixPool<NODE>       PoolType; //!< aliases
        typedef Memory::ReadOnlyBuffer PathType; //!< aliases

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        inline explicit SuffixTree() : list(), pool(), tree() {} //!< setup empty
        inline virtual ~SuffixTree() noexcept { hardReset(); }   //!< cleanup
        inline          SuffixTree(const SuffixTree &other) :
        list(), pool(), tree() { duplicate_(other); }            //!< copy

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        //______________________________________________________________________
        //
        //! insertion for Map
        //______________________________________________________________________
        inline bool insert(ParamKey key, ParamType t)
        { return insert_( pool.template create<KEY,T>(key,t) ); }

        //______________________________________________________________________
        //
        //! insertion for Setp
        //______________________________________________________________________
        bool insert(ParamType t)
        { return insert_( pool.template create<T>(t) ); }


    protected:
        ListType         list; //!< managed list of Nodes
        PoolType         pool; //!< memory pool
        Core::SuffixTree tree; //!< internal tree

        
        //______________________________________________________________________
        //
        //! release all possible memory
        //______________________________________________________________________
        inline void hardReset() noexcept {
            tree.release();
            while(list.size>0)
                pool.vaporize(list.popTail());
            pool.release();
        }

        //______________________________________________________________________
        //
        //! reset content, keep memory
        //______________________________________________________________________
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

        inline void duplicate_(const SuffixTree &other)
        {
            for(const NodeType *node=other.list.head;node;node=node->next)
            {
                if(!insert_( pool.duplicate(node) ) )
                    tree.unexpectedCopyException();
            }
        }

    };
}

#endif

