
//! \file

#ifndef Y_Associative_Hash_Linked_Included
#define Y_Associative_Hash_Linked_Included 1

#include "y/associative/hash/table.hpp"

namespace Yttrium
{

    template <
    typename KEY,
    typename T,
    typename NODE,
    typename KEY_HASHER>
    class HashLinked
    {
    public:
        Y_ARGS_DECL(T,Type);
        Y_ARGS_DECL(KEY,Key);
        typedef Blanks<NODE> NodePool;

        inline explicit HashLinked() : nodes(), table(0), kpool(0), npool(0) {}
        inline virtual ~HashLinked() noexcept { release_(); }

        inline bool insert_(ParamKey k, ParamType t)
        {
            // create a built node
            NODE *node = npool.queryBlank();
            try { node = new (node) NODE(k,t); }
            catch(...) { npool.storeBlank(node); throw; }

            // insert it
            return insert__(node);
        }

        inline bool insert_(ParamType t)
        {
            // create a built node
            NODE *node = npool.queryBlank();
            try { node = new (node) NODE(t); }
            catch(...) { npool.storeBlank(node); throw; }

            // insert it
            return insert__(node);
        }



    protected:
        ListOf<NODE>   nodes; //!< actual data
        HashTable      table; //!< current table
        HashKnot::Pool kpool; //!< pool of knots
        NodePool       npool; //!< pool of nodes
        KEY_HASHER     hashr; //!< key hasher

        inline void release_() noexcept {
            table.releaseWith(kpool);
            while(nodes.size>0)
                npool.storeBuilt(nodes.popTail());
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(HashLinked);
        inline bool insert__(NODE *node)
        {
            try {
                const size_t    hkey = hashr(node->key); // get hash key
                HashKnot::List &slot = table[hkey];      // get hash slot

                // check if key exist
                for(const HashKnot *knot=slot.head;knot;knot=knot->next)
                {
                    //  starting by hash keys comparison
                    if(knot->hkey==hkey)
                    {
                        // then full key comparison
                        assert(0!=knot->node);
                        const NODE *curr = static_cast<const NODE *>(knot->node);
                        assert(nodes.owns(curr));
                        if(curr->key == node->key)
                        {
                            npool.storeBuilt(node);
                            return false;
                        }
                    }
                }
                slot.pushHead( new (kpool.queryBlank()) HashKnot(hkey,node) );
                nodes.pushTail(node);
                return true;
            }
            catch(...) { npool.storeBuilt(node); throw; }
        }
    };

}

#endif

