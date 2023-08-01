#include "y/associative/suffix/tree.hpp"
#include "y/data/list.hpp"
#include "y/memory/blanks.hpp"
#include "y/object.hpp"
#include "y/type/destruct.hpp"
#include "y/exception.hpp"
#include "y/text/hexadecimal.hpp"

namespace Yttrium
{
    namespace
    {
        class SuffixNode
        {
        public:
            inline SuffixNode(SuffixNode *  parent,
                              const uint8_t encode) noexcept :
            code(encode), data(0), from(parent), next(0), prev(0), chld()
            {
                std::cerr << "create node @" << Hexadecimal(code) << std::endl;
            }

            inline ~SuffixNode() noexcept {}

            const uint8_t      code;
            void              *data;
            SuffixNode *       from;
            SuffixNode *       next;
            SuffixNode *       prev;
            ListOf<SuffixNode> chld;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(SuffixNode);
        };

    }

    class SuffixTree::Code : public Object
    {
    public:
        void destroy(SuffixNode *node)
        {
            assert(0!=node);
            while(node->chld.size>0)
            {
                destroy(node->chld.popTail());
            }
            pool.zrelease( Destructed(node) );
        }
        explicit Code() : root(0), pool(0)
        {
            root = new (pool.zacquire()) SuffixNode(0,0);
        }

        virtual ~Code() noexcept
        {
            assert(0!=root);
            destroy(root);
            root = 0;
        }


        SuffixNode *queryNext(SuffixNode *curr,const uint8_t code)
        {
            ListOf<SuffixNode> &chld = curr->chld;
            switch(curr->chld.size)
            {
                case 0: return (curr = chld.pushTail( new (pool.zacquire()) SuffixNode(curr,code) ));
                case 1:
                    assert(0!=chld.head);
                    switch( Sign::Of(code,chld.head->code))
                    {
                        case Negative: return (curr = chld.pushHead( new (pool.zacquire()) SuffixNode(curr,code) ));
                        case __Zero__: return (curr = chld.head);
                        case Positive: return (curr = chld.pushTail( new (pool.zacquire()) SuffixNode(curr,code) ) );
                    }
                    // never get here
                    break;

                default:
                    break;
            }
            assert(chld.size>=2);
            SuffixNode *lower = chld.head; assert(0!=lower);
            switch( Sign::Of(code,lower->code) )
            {
                case Negative: return (curr = chld.pushHead( new (pool.zacquire()) SuffixNode(curr,code) ));
                case __Zero__: return (curr = lower);
                case Positive: break;
            }

            SuffixNode *upper = chld.tail;
            switch ( Sign::Of(code,upper->code)) {
                case Negative: break;
                case __Zero__: return (curr = lower);
                case Positive: return (curr = chld.pushTail( new (pool.zacquire()) SuffixNode(curr,code) ) );
            }

            assert(code>lower->code);
            assert(code<upper->code);

            throw Exception("not implemented");
        }

        void * insert(const uint8_t *path, const size_t size, void *data)
        {
            assert( Good(path,size) );

            // initialize
            SuffixNode  *curr = root; assert(0!=curr);

            // walk down building the path
            for(size_t i=0;i<size;++i)
                curr = queryNext(curr,path[i]);

            // done
            if(0==curr->data)
            {
                curr->data = data;
                return curr;
            }
            else
            {
                // occupied
                return 0;
            }
        }

        SuffixNode        *root;
        Blanks<SuffixNode> pool;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Code);
    };

    SuffixTree:: SuffixTree() : code( new Code() )
    {
        std::cerr << "sizeof(SuffixNode)=" << sizeof(SuffixNode) << std::endl;
        std::cerr << "sizeof(Code)=" << sizeof(Code) << std::endl;

    }

    SuffixTree:: ~SuffixTree() noexcept
    {
        assert(0!=code);
        delete code;
        code = 0;
    }

    void * SuffixTree:: insert(const void *path, const size_t size, void *data)
    {
        assert(Good(path,size));
        return code->insert( static_cast<const uint8_t*>(path),size, data);
    }

    void * SuffixTree:: insert(const Memory::ReadOnlyBuffer &buff, void *data)
    {
        return insert( buff.ro_addr(), buff.measure(), data);
    }


}
