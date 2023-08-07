
//! \file

#ifndef Y_Associative_Hash_Linked_Included
#define Y_Associative_Hash_Linked_Included 1

#include "y/associative/hash/table.hpp"
#include "y/container/iterator/linked.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{

    namespace Core
    {
        //______________________________________________________________________
        //
        //! helper for HashLinked
        //______________________________________________________________________
        struct HashLinked
        {
            //! raise exception
            static void UnexpectedInsertFailure(const char *);
        };
    }

    //__________________________________________________________________________
    //
    //
    //
    //! Composite class for Hash[Map|Set]
    //
    //
    //__________________________________________________________________________
    template <
    typename KEY,
    typename T,
    class    INTERFACE,
    typename NODE,
    typename KEY_HASHER,
    class    BASE>
    class HashLinked : public INTERFACE, public BASE
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        Y_ARGS_DECL(T,Type);                                     //!< aliases
        Y_ARGS_DECL(KEY,Key);                                    //!< aliases
        typedef Blanks<NODE> NodePool;                           //!< alias
        static const size_t  LoadFactor = HashTable::LoadFactor; //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        //! cleanup
        inline virtual ~HashLinked() noexcept { release_(); }

        //! create with initial capacity
        inline explicit HashLinked(const size_t nmin) :
        INTERFACE(), BASE(), nodes(), table(nmin/LoadFactor), kpool(nmin), npool(nmin), hashr() {}

        //! duplicate
        inline HashLinked(const HashLinked &other) :
        Identifiable(), Collection(), INTERFACE(), BASE(),
        nodes(),
        table(other.nodes.size/LoadFactor),
        kpool(other.nodes.size),
        npool(other.nodes.size),
        hashr()
        {
            duplicate(other);
        }

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________


        inline virtual size_t       size()      const noexcept { return nodes.size;                   }  //!< size
        inline virtual const char * callSign()  const noexcept { return BASE::CallSign;               }  //!< CallSign
        inline virtual size_t       capacity()  const noexcept { return kpool.available()+nodes.size; }  //!< capacity
        inline virtual size_t       available() const noexcept { return kpool.available();            }  //!< available
        inline virtual void         free()            noexcept { free_(); }                              //!< free, keep memory
        inline virtual void         release()         noexcept { release_(); }                           //!< free, release memory
        inline virtual void         reserve(size_t n)          { reserve_(n); }                          //!< reserve nodes/knots
        inline virtual bool         remove(ParamKey key)       noexcept { return remove_(key); }         //!< remove by key
        inline virtual ConstType *  search(ParamKey key) const noexcept { return search_(key); }         //!< search by key
        inline virtual Type *       search(ParamKey key)       noexcept { return (Type *)search_(key); } //!< search by key


        //______________________________________________________________________
        //
        //
        // Iterators
        //
        //______________________________________________________________________

        typedef Iterating::Linked<Type,NODE,Iterating::Forward>            Iterator;      //!< forward
        typedef Iterating::Linked<ConstType,const NODE,Iterating::Forward> ConstIterator; //!< forward const

        inline Iterator      begin()       noexcept { return Iterator(nodes.head);      }  //!< begin forward
        inline Iterator      end()         noexcept { return Iterator(0);               }  //!< end   forward
        inline ConstIterator begin() const noexcept { return ConstIterator(nodes.head); }  //!< begin const forward
        inline ConstIterator end()   const noexcept { return ConstIterator(0);          }  //!< end   const forward


        //______________________________________________________________________
        //
        //
        // Members
        //
        //______________________________________________________________________


    protected:
        ListOf<NODE>       nodes; //!< actual data
        HashTable          table; //!< current table
        HashKnot::Pool     kpool; //!< pool of knots
        NodePool           npool; //!< pool of nodes
        mutable KEY_HASHER hashr; //!< key hasher


        //! insert for map
        inline bool insert_(ParamKey k, ParamType t)
        {
            // create a built node
            NODE *node = npool.queryBlank();
            try { node = new (node) NODE(k,t); }
            catch(...) { npool.storeBlank(node); throw; }

            // insert it
            return insert__(node);
        }

        //! insert for set
        inline bool insert_(ParamType t)
        {
            // create a built node
            NODE *node = npool.queryBlank();
            try { node = new (node) NODE(t); }
            catch(...) { npool.storeBlank(node); throw; }

            // insert it
            return insert__(node);
        }

    private:
        Y_DISABLE_ASSIGN(HashLinked);


        //! free content, keep memory
        inline void free_() noexcept
        {
            table.freeWith(kpool);
            while(nodes.size>0)
                npool.storeBuilt(nodes.popTail());
        }

        //! free content, release memory
        inline void release_() noexcept {
            table.releaseWith(kpool);
            while(nodes.size>0)
                npool.eraseBuilt(nodes.popTail());
        }

        //! reserve matching
        inline void reserve_(size_t n)
        {
            try {
                kpool.reserve(n);
                npool.reserve(n);
            }
            catch(...)
            {
                const size_t nmax = Min(kpool.available(),npool.available());
                kpool.gc(nmax);
                npool.gc(nmax);
                throw;
            }

        }

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


        inline void duplicate(const HashLinked &other)
        {
            for(const NODE *scan=other.nodes.head;scan;scan=scan->next)
            {
                NODE *node = npool.queryBlank();
                try { node = new (node) NODE(*scan); }
                catch(...) { npool.storeBlank(node); throw; }
                if(!insert__(node))
                    Core::HashLinked::UnexpectedInsertFailure(callSign());
            }
        }

        inline ConstType *search_(ConstKey &key) const noexcept
        {
            const size_t          hkey = hashr(key);
            const HashKnot::List &slot = table[hkey];
            for(const HashKnot *knot=slot.head;knot;knot=knot->next)
            {
                if(hkey!=knot->hkey) continue; // fast pre-test
                assert(0!=knot->node);
                NODE *node = static_cast<NODE *>(knot->node);
                if(key == node->key)
                    return & **node;
            }
            return 0;
        }


        inline bool remove_(ConstKey &key) noexcept
        {
            const size_t    hkey = hashr(key);
            HashKnot::List &slot = table[hkey];
            for(HashKnot *knot=slot.head;knot;knot=knot->next)
            {
                if(hkey!=knot->hkey) continue; // fast pre-test
                assert(0!=knot->node);
                NODE *node = static_cast<NODE *>(knot->node);
                if(key == node->key)
                {
                    kpool.storeBuilt(slot.pop(knot));
                    npool.storeBuilt(nodes.pop(node));
                    return true;
                }
            }
            return false;

        }


    };

}

#endif

