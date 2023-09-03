#include "y/associative/suffix/core/tree.hpp"
#include "y/data/list.hpp"
#include "y/memory/blanks.hpp"
#include "y/object.hpp"
#include "y/system/exception.hpp"
#include "y/graphviz/vizible.hpp"

namespace Yttrium
{
    namespace Core
    {
        namespace
        {
            class                      SuffixNode;
            typedef ListOf<SuffixNode> SuffixList;

            //__________________________________________________________________
            //
            //
            //! SuffixNode, holding information for SuffixTree
            //
            //__________________________________________________________________
            class SuffixNode
            {
            public:

                //______________________________________________________________
                //
                //! setup
                //______________________________________________________________
                inline SuffixNode(SuffixNode *  parent,
                                  const uint8_t encode) noexcept :
                code(encode), data(0), from(parent), next(0), prev(0), chld()
                {
                }

                //______________________________________________________________
                //
                //! cleanup
                //______________________________________________________________
                inline ~SuffixNode() noexcept {}

                const uint8_t code; //!< local code
                void *        data; //!< local data, NULL <=> free
                SuffixNode *  from; //!< for link
                SuffixNode *  next; //!< for list
                SuffixNode *  prev; //!< for list
                SuffixList    chld; //!< for list

                //______________________________________________________________
                //
                //! GraphViz
                //______________________________________________________________
                inline void viz(OutputStream &fp) const
                {
                    Vizible::Addr(fp,this) << '[';
                    if(0==from)
                    {
                        Vizible::Label(fp, "");
                        fp << ",shape=triangle";
                    }
                    else
                    {
                        Vizible::Label(fp,&code,1);
                        fp << ",shape=circle";
                    }
                    if(data)
                    {
                        fp << ",style=bold";
                    }
                    else
                    {
                        fp << ",style=dotted";
                    }
                    fp  << ']';
                    Vizible::Endl(fp);
                    for(const SuffixNode *node=chld.head;node;node=node->next)
                    {
                        // node
                        node->viz(fp);

                        // link
                        Vizible::Addr(fp,this);
                        fp << "->";
                        Vizible::Addr(fp,node);
                        Vizible::Endl(fp);
                    }
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(SuffixNode);
            };

        }

        //______________________________________________________________________
        //
        //
        //! Effective tree
        //
        //__________________________________________________________________
        class SuffixTree::Code : public Object
        {
        public:

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! create a root from the pool
            inline explicit Code() : root(0), pool(0)
            {
                root = new (pool.queryBlank()) SuffixNode(0,0);
            }

            //! destroy the root
            virtual ~Code() noexcept
            {
                assert(0!=root);
                expunge(root);
                root = 0;
            }


            //__________________________________________________________________
            //
            //
            // methods
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //! recursive destruct, keep memory in pool
            //__________________________________________________________________
            inline void destroy(SuffixNode *node) noexcept
            {
                assert(0!=node);
                while(node->chld.size>0)
                {
                    destroy(node->chld.popTail());
                }
                pool.storeBuilt( node );
            }

            //__________________________________________________________________
            //
            //! reset to empty, keep memory in pool
            //__________________________________________________________________
            inline void free() noexcept
            {
                assert(0!=root);
                root->data = 0;
                while(root->chld.size)
                {
                    destroy(root->chld.popTail());
                }
            }

            //__________________________________________________________________
            //
            //! recursive expunge
            //__________________________________________________________________
            inline void expunge(SuffixNode *node) noexcept
            {
                assert(0!=node);
                while(node->chld.size>0)
                {
                    expunge(node->chld.popTail());
                }
                pool.eraseBuilt( node );
            }

            //__________________________________________________________________
            //
            //! reset to empty, discard memory
            //__________________________________________________________________
            inline void release() noexcept
            {
                root->data = 0;
                while(root->chld.size)
                {
                    expunge(root->chld.popTail());
                }
            }


            //__________________________________________________________________
            //
            //! query next node by finding matching code or creating a new one
            //__________________________________________________________________
            SuffixNode *queryNext(SuffixNode *curr,const uint8_t code)
            {
                ListOf<SuffixNode> &chld = curr->chld;
                switch(curr->chld.size)
                {
                    case 0: return (curr = chld.pushTail( new (pool.queryBlank()) SuffixNode(curr,code) ));
                    case 1:
                        assert(0!=chld.head);
                        switch( Sign::Of(code,chld.head->code))
                        {
                            case Negative: return (chld.pushHead( new (pool.queryBlank()) SuffixNode(curr,code) ));
                            case __Zero__: return (chld.head);
                            case Positive: return (chld.pushTail( new (pool.queryBlank()) SuffixNode(curr,code) ) );
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
                    case Negative: return (chld.pushHead( new (pool.queryBlank()) SuffixNode(curr,code) ));
                    case __Zero__: return (lower);
                    case Positive: break;
                }

                SuffixNode * const upper = chld.tail;
                switch ( Sign::Of(code,upper->code)) {
                    case Negative: break;
                    case __Zero__: return (upper);
                    case Positive: return (chld.pushTail( new (pool.queryBlank()) SuffixNode(curr,code) ) );
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
                return chld.insertAfter(lower, new (pool.queryBlank()) SuffixNode(curr,code) );
            }

            //__________________________________________________________________
            //
            //! insert data at a given path
            //__________________________________________________________________
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
                    return 0; // occupied
                }
            }

            //__________________________________________________________________
            //
            //! search data
            //__________________________________________________________________
            const void *search(const uint8_t *path, const size_t size) const noexcept
            {
                assert(Good(path,size));
                const SuffixNode *curr = root;
                for(size_t i=0;i<size;++i)
                {
                    assert(0!=curr);
                    const SuffixList &chld = curr->chld; if(chld.size<=0) return 0;
                    const uint8_t     code = path[i];
                    assert(chld.head!=0); if(code<chld.head->code) return 0;
                    assert(chld.tail!=0); if(code>chld.tail->code) return 0;
                    curr = chld.head;

                LOOP:
                    if(code==curr->code)
                        continue;
                    curr = curr->next;
                    if(!curr) return 0;
                    goto LOOP;
                }
                return curr->data;
            }

            //__________________________________________________________________
            //
            //! prune
            //__________________________________________________________________
            inline void prune(SuffixNode *node) noexcept
            {
                assert(0!=node);
                if(0==node->data && 0!=node->from && node->chld.size<=0)
                {
                    destroy(node->from->chld.pop(node));
                    prune(node->from);
                }
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            SuffixNode        *root;
            Blanks<SuffixNode> pool;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };

        SuffixTree:: SuffixTree() : code( new Code() )
        {
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
            assert(0!=data);
            return code->insert( static_cast<const uint8_t*>(path),size, data);
        }

        void * SuffixTree:: insert(const Memory::ReadOnlyBuffer &buff, void *data)
        {
            return insert( buff.ro_addr(), buff.measure(), data);
        }

        Yttrium::String SuffixTree:: pathOf(const void *nodeAddr) const
        {
            assert(0!=nodeAddr);
            const SuffixNode *node = static_cast<const SuffixNode *>( nodeAddr );
            Yttrium::String   path;
            while(0!=node->from)
            {
                path << node->code;
                node=node->from;
            }
            path.reverse();
            return path;
        }

        bool SuffixTree:: owns(const void *nodeAddr) const noexcept
        {
            assert(0!=nodeAddr);
            const SuffixNode *node = static_cast<const SuffixNode *>( nodeAddr );
            while(0!=node->from)
            {
                node = node->from;
            }
            assert(0!=code);
            return node == code->root;
        }

        const void  * SuffixTree:: search(const void *path, const size_t size) const noexcept
        {
            assert(Good(path,size));
            assert(0!=code);
            return code->search(static_cast<const uint8_t*>(path),size);
        }

        const void  *SuffixTree:: search(const Memory::ReadOnlyBuffer &buff) const noexcept
        {
            return search( buff.ro_addr(), buff.measure() );

        }

        void SuffixTree:: loosen(void *nodeAddr) noexcept
        {
            assert(0!=nodeAddr);
            assert(0!=code);
            assert(owns(nodeAddr));
            SuffixNode *node = static_cast<SuffixNode *>( nodeAddr );
            node->data = 0;
            code->prune(node);
        }

        void SuffixTree:: free() noexcept
        {
            assert(0!=code);
            code->free();
        }

        void SuffixTree:: trim() noexcept
        {
            assert(0!=code);
            code->pool.release();
            assert(0==code->pool.available());
        }

        void SuffixTree:: release() noexcept
        {
            assert(0!=code);
            code->release();
        }

        void SuffixTree:: unexpectedCopyException() const
        {
            throw Specific::Exception("SuffixTree(copy)","unexpected insertion failure!");
        }

        void SuffixTree:: graphViz(OutputStream &fp) const
        {
            assert(0!=code);
            assert(0!=code->root);
            Vizible::Enter(fp,"G");
            code->root->viz(fp);
            Vizible::Leave(fp);
        }

        

    }

}
