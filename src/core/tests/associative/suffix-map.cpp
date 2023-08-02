
#include "y/associative/suffix/tree.hpp"
#include "y/associative/suffix/pool.hpp"

#include "y/io/libc/input.hpp"
#include "y/string.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"


using namespace Yttrium;

namespace Yttrium
{

  


    template <typename KEY, typename T>
    class SuffixMapNode
    {
    public:
        Y_ARGS_DECL(T,Type);
        Y_ARGS_DECL(KEY,Key);

        inline   SuffixMapNode(ParamKey k, ParamType t) :
        next(0), prev(0), knot(0),
        data(t), key(k)
        {
        }

        inline SuffixMapNode(const SuffixMapNode &node) :
        next(0), prev(0), knot(0),
        data(node.data), key(node.key)
        {
        }


        inline ~SuffixMapNode() noexcept {}

        SuffixMapNode *next;
        SuffixMapNode *prev;
        void          *knot;
        MutableType    data;
        ConstKey       key;

    private:
        Y_DISABLE_ASSIGN(SuffixMapNode);
    };

    template <typename KEY, typename T>
    class SuffixSetNode
    {
    public:
        Y_ARGS_DECL(T,Type);
        Y_ARGS_DECL(KEY,Key);

        inline   SuffixSetNode(ParamType t) :
        next(0), prev(0), knot(0),
        data(t), key(data.key())
        {
        }

        inline ~SuffixSetNode() noexcept {}

        inline SuffixSetNode(const SuffixSetNode &node) :
        next(0), prev(0), knot(0),
        data(node.data), key(data.key())
        {
        }

        SuffixSetNode *next;
        SuffixSetNode *prev;
        void          *knot;
        MutableType    data;
        ConstKey      &key;

    private:
        Y_DISABLE_ASSIGN(SuffixSetNode);
    };


    class Dummy
    {
    public:
        Dummy(const char *id) : name(id)
        {
        }
        ~Dummy() noexcept {}

        Dummy(const Dummy &other) : name(other.name) {}

        const String &key() const noexcept { return name; }

        const String name;

    private:
        Y_DISABLE_ASSIGN(Dummy);
    };



    template <class KEY, class T, class NODE>
    class SuffixTree
    {
    public:
        Y_ARGS_DECL(T,Type);
        Y_ARGS_DECL(KEY,Key);
        typedef NODE NodeType;
        typedef ListOf<NODE>     ListType;
        typedef SuffixPool<NODE> PoolType;
        typedef Memory::ReadOnlyBuffer PathType;

        explicit SuffixTree() : list(), pool(), tree() {}
        virtual ~SuffixTree() noexcept {}

        bool insert(ParamKey key, ParamType t)
        {
            return insert_( pool.template create<KEY,T>(key,t) );
        }

        bool insert(ParamType t)
        {
            return insert_( pool.template create<T>(t) );
        }


        ListType         list;
        PoolType         pool;

    private:
        Core::SuffixTree tree;
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
                return true;
            }
            catch(...) { pool.destruct(node); throw; }
        }

    };

}

Y_UTEST(associative_suffix_map)
{
    {
        typedef SuffixMapNode<String,int>   MapNode;
        typedef SuffixSetNode<String,Dummy> SetNode;

        SuffixPool<MapNode> mapPool;
        {
            MapNode *node = mapPool.create<String,int>("Hello",1);
            MapNode *copy = mapPool.duplicate(node);
            mapPool.destruct(node);
            mapPool.destruct(copy);

        }

        SuffixPool<SetNode> setPool;
        {
            const Dummy dum("World");
            SetNode *node = setPool.create<Dummy>(dum);
            SetNode *copy = setPool.duplicate(node);
            setPool.destruct(node);
            setPool.destruct(copy);
        }
    }

    SuffixTree<String,int, SuffixMapNode<String,int> > mapTree;
    Y_CHECK(mapTree.insert("Hello",1));


}
Y_UDONE()
