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
        //! SuffixNode, holding information for SuffixTree
        class SuffixNode
        {
        public:
            inline SuffixNode(SuffixNode *  parent,
                              const uint8_t encode) noexcept :
            code(encode), data(0), from(parent), next(0), prev(0), chld()
            {
            }

            inline ~SuffixNode() noexcept {}

            const uint8_t      code; //!< local code
            void              *data; //!< local data, NULL <=> free
            SuffixNode *       from; //!< for link
            SuffixNode *       next; //!< for list
            SuffixNode *       prev; //!< for list
            ListOf<SuffixNode> chld; //!< for list

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
                        case Negative: return (chld.pushHead( new (pool.zacquire()) SuffixNode(curr,code) ));
                        case __Zero__: return (chld.head);
                        case Positive: return (chld.pushTail( new (pool.zacquire()) SuffixNode(curr,code) ) );
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
                case Negative: return (chld.pushHead( new (pool.zacquire()) SuffixNode(curr,code) ));
                case __Zero__: return (lower);
                case Positive: break;
            }

            SuffixNode * const upper = chld.tail;
            switch ( Sign::Of(code,upper->code)) {
                case Negative: break;
                case __Zero__: return (upper);
                case Positive: return (chld.pushTail( new (pool.zacquire()) SuffixNode(curr,code) ) );
            }

            assert(0!=lower);
            assert(0!=upper);
            assert(code>lower->code);
            assert(code<upper->code);

            while(lower->next!=upper)
            {
                SuffixNode * const probe = lower->next;

                switch( Sign::Of(code,probe->code))
                {
                    case Negative: goto DONE;
                    case __Zero__: return (probe);
                    case Positive: break;
                }
                lower=probe;
            }
        DONE:
            assert(0!=lower);
            assert(code>lower->code);
            assert(0!=lower->next);
            assert(code<lower->next->code);
            return chld.insertAfter(lower, new (pool.zacquire()) SuffixNode(curr,code) );
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

    String SuffixTree:: pathOf(const void *nodeAddr) const
    {
        assert(0!=nodeAddr);
        const SuffixNode *node = static_cast<const SuffixNode *>( nodeAddr );
        String            path;
        while(0!=node->from)
        {
            path << node->code;
            node=node->from;
        }
        //std::cerr << "[path=" << path << "]" << std::endl;
        path.reverse();
        return path;
    }


}
