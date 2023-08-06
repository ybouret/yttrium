
#include "y/memory/blanks.hpp"
#include "y/calculus/align.hpp"
#include "y/concurrent/memory.hpp"
#include "y/lockable.hpp"
#include "y/memory/blocks.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/data/rework.hpp"
#include "y/sort/merge.hpp"
#include "y/object.hpp"

namespace Yttrium
{
    namespace Memory
    {

        const char * const Blanks:: CallSign = "Memory::Blanks";

        namespace
        {
            //__________________________________________________________________
            //
            //
            //! alias to internally handled data blocks
            //
            //__________________________________________________________________
            struct BlankNode
            {
                typedef PoolOf<BlankNode> Pool; //!< alias
                BlankNode *next;                //!< for pool/list
                BlankNode *prev;                //!< for list
            };

        }
        //______________________________________________________________________
        //
        //! check block size, aligned and fit a blank node
        //______________________________________________________________________
        static inline size_t BlockSizeFor(const size_t bs)
        {
            if(bs<=sizeof(BlankNode))
            {
                return sizeof(BlankNode);
            }
            else
            {
                return Y_MEMALIGN(bs);
            }
        }

        //______________________________________________________________________
        //
        //! get matching arena, with locking
        //______________________________________________________________________
        static Arena & GetArena(const size_t blockSize,
                                Lockable    &giantLock)
        {
            Y_LOCK(giantLock);
            return Concurrent::Mem::BlocksInstance()[blockSize];
        }

        //______________________________________________________________________
        //
        //
        //! handling blanks
        //
        //______________________________________________________________________
        class Blanks:: Code : public BlankNode::Pool
        {
        public:

            //__________________________________________________________________
            //
            //! setup from parameters
            //__________________________________________________________________
            explicit Code(const size_t userBlockSize,
                          const size_t startCapacity) :
            allocated(0),
            giantLock( Lockable::Giant() ),
            coreArena( GetArena(BlockSizeFor(userBlockSize),giantLock) )
            {
                try { reserve(startCapacity); }
                catch(...) { empty(); throw;  }
            }

            //__________________________________________________________________
            //
            //! cleanup
            //__________________________________________________________________
            inline virtual ~Code() noexcept {
                if(allocated)
                    std::cerr << "[" << CallSign << "] : blockSize=" << coreArena.blockSize << " #allocated=" << allocated << std::endl;
                empty();
            }

            //__________________________________________________________________
            //
            //! lock and store blocks
            //__________________________________________________________________
            inline void reserve(size_t n)
            {
                Y_LOCK(giantLock);
                while(n-- > 0) store( static_cast<BlankNode *>( coreArena.acquireBlock()) );
            }

            //__________________________________________________________________
            //
            //! lock and empty pool
            //__________________________________________________________________
            inline void empty() noexcept
            {
                Y_LOCK(giantLock);
                while(size>0)
                    coreArena.releaseBlock( query() );
            }

            //__________________________________________________________________
            //
            //! fetch an existing or a new block
            //__________________________________________________________________
            inline void *fetchBlank()
            {
                if(size>0)
                {
                    ++allocated;
                    return OutOfReach::Zero( query(), coreArena.blockSize);
                }
                else
                {
                    Y_LOCK(giantLock);
                    void *blockAddr = coreArena.acquireBlock();
                    ++allocated;
                    return blockAddr;
                }
            }

            //__________________________________________________________________
            //
            //! store within pool
            //__________________________________________________________________
            inline void storeBlank(void *blockAddr) noexcept
            {
                assert(0!=blockAddr);
                assert(allocated>0);
                --allocated;
                store( static_cast<BlankNode *>( OutOfReach::Zero(blockAddr,sizeof(BlankNode))) );
            }

            //__________________________________________________________________
            //
            //! back to arena
            //__________________________________________________________________
            inline void eraseBlank(void *blockAddr) noexcept
            {
                assert(0!=blockAddr);
                assert(allocated>0);
                --allocated;
                Y_GIANT_LOCK();
                coreArena.releaseBlock(blockAddr);
            }

            //__________________________________________________________________
            //
            //! GC
            //__________________________________________________________________
            inline void gc(const size_t maxCount) noexcept
            {
                ListOf<BlankNode> data;
                Rework::PoolToList(data,*this);               // get data
                MergeSort::ByIncreasingAddress(data);         // sort by increasing address

                {
                    Y_GIANT_LOCK();
                    while(data.size>maxCount)
                        coreArena.releaseBlock( data.popTail() ); // return highest
                }
                while(data.size) store( data.popHead() );         // will use highest first
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            size_t       allocated; //!< bookkeeping
            Lockable    &giantLock; //!< to access coreArena
            Arena       &coreArena; //!< Quark's arena

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };


        Blanks:: Blanks(const size_t userBlockSize,
                        const size_t startCapacity) :
        code( new Code(userBlockSize,startCapacity) )
        {

        }



        void Blanks:: release() noexcept
        { assert(0!=code); code->empty(); }

        Blanks:: ~Blanks() noexcept
        {
            assert(0!=code);
            delete code;
            code = 0;
        }

        void   Blanks:: reserve(const size_t n)
        { assert(0!=code); code->reserve(n); }


        void * Blanks:: fetchBlank()
        {
            assert(0!=code);
            return code->fetchBlank();
        }

        void Blanks:: storeBlank(void *blockAddr) noexcept
        {
            assert(0!=code);
            code->storeBlank(blockAddr);
        }

        void Blanks:: eraseBlank(void *blockAddr) noexcept
        {
            assert(0!=code);
            code->eraseBlank(blockAddr);
        }


        size_t Blanks:: blockSize() const noexcept
        {
            assert(code!=0);
            return code->coreArena.blockSize;
        }

        size_t Blanks:: available() const noexcept
        {
            assert(code!=0);
            return code->size;
        }

        size_t Blanks:: allocated() const noexcept
        {
            assert(code!=0);
            return code->allocated;
        }


        
        void Blanks:: displayInfo(const size_t indent) const
        {
            static const char id[] = "Memory::Blanks";
            Core::Indent(std::cerr,indent) << "<" << id << " blockSize='" << blockSize() << "'>" << std::endl;
            Core::Indent(std::cerr,indent) << "  allocated = " << allocated() << std::endl;
            Core::Indent(std::cerr,indent) << "  available = " << available() << std::endl;
            Core::Indent(std::cerr,indent) << "<" << id << "/>" << std::endl;
        }


        void Blanks:: gc(const size_t maxCount) noexcept
        {
            assert(code!=0);
            code->gc(maxCount);
        }


    }
}

