#include "y/container/operating.hpp"
#include "y/object.hpp"
#include "y/type/nullify.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Core
    {
        class Operating:: Code : public Object
        {
        public:
            void * const entry;
            const size_t width;
            size_t       built;
            Smash const  erase;

            inline explicit Code(void *       blockAddr,
                                 const size_t numBlocks,
                                 const size_t blockSize,
                                 Build        build,
                                 void        *param,
                                 Smash        smash) :
            Object(),
            entry(blockAddr),
            width(blockSize),
            built(0),
            erase(smash)
            {
                assert(Good(blockAddr,numBlocks));
                assert(blockSize>0);
                assert(0!=erase);
                assert(0!=build);
                assert(width>0);
                
                try {
                    uint8_t *addr = static_cast<uint8_t *>(entry);
                    while(built<numBlocks) {
                        build(addr,param);
                        addr += width;
                        ++built;
                    }

                }
                catch(...)
                {
                    release();
                    throw;
                }
            }



            inline explicit Code(void        *blockAddr,
                                 const size_t blockSize,
                                 const Code  *source,
                                 XCopy        xcopy,
                                 Smash        smash) :
            Object(),
            entry(blockAddr),
            width(blockSize),
            built(0),
            erase(smash)
            {
                assert(blockSize>0);
                assert(0!=erase);
                assert(0!=xcopy);
                assert(width>0);

                try {
                    uint8_t       *tgt = static_cast<uint8_t       *>(entry);
                    const uint8_t *src = static_cast<const uint8_t *>(source->entry);
                    while(built<source->built) {
                        xcopy(tgt,src);
                        tgt += width;
                        src += source->width;
                        ++built;
                    }

                }
                catch(...)
                {
                    release();
                    throw;
                }
            }

            inline explicit Code(void        *blockAddr,
                                 const size_t numBlocks,
                                 const size_t blockSize,
                                 XProc        xproc,
                                 void        *param,
                                 Smash        smash) :
            Object(),
            entry(blockAddr),
            width(blockSize),
            built(0),
            erase(smash)
            {
                assert(Good(blockAddr,numBlocks));
                assert(blockSize>0);
                assert(0!=erase);
                assert(0!=xproc);
                assert(0!=param);
                assert(width>0);

                //std::cerr << "Build #" << numBlocks << " with Procedural Code, entry@" << entry << ", width=" << width << std::endl;

                try {
                    uint8_t       *tgt = static_cast<uint8_t *>(entry);
                    while(built<numBlocks) {
                        xproc(tgt,param,built+1);
                        tgt += width;
                        ++built;
                    }
                }
                catch(...)
                {
                    release();
                    throw;
                }
            }



            inline virtual ~Code() noexcept { release(); }

            inline void release() noexcept
            {
                assert(0!=erase);
                assert(width>0);

                uint8_t *addr = static_cast<uint8_t *>(entry);
                size_t   size = built*width;
                while(size>0)
                {
                    assert(0!=addr);
                    erase( &addr[size -= width] );
                }
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };


        Operating:: Operating(void *       blockAddr,
                              const size_t numBlocks,
                              const size_t blockSize,
                              Build        build,
                              void        *param,
                              Smash        smash) :
        code( new Code(blockAddr,numBlocks,blockSize,build,param,smash) )
        {

        }

        Operating:: Operating(void *           blockAddr,
                              const size_t     blockSize,
                              const Operating &source,
                              XCopy            xcopy,
                              Smash            smash) :
        code( new Code(blockAddr,blockSize,source.code,xcopy,smash) )
        {
        }

        Operating:: Operating(void *       blockAddr,
                              const size_t numBlocks,
                              const size_t blockSize,
                              XProc        xproc,
                              void        *param,
                              Smash        smash) :
        code( new Code(blockAddr,numBlocks,blockSize,xproc,param,smash) )
        {

        }


        Operating:: ~Operating() noexcept
        {
            assert(code);
            Nullify(code);
        }

        const void * Operating:: entry() const noexcept
        {
            assert(0!=code);
            return code->entry;
        }

        size_t Operating:: blocks() const noexcept
        {
            assert(0!=code);
            return code->built;
        }

        void Operating:: swapCodeWith(Operating &op) noexcept
        {
            Swap(code,op.code);
        }

    }
}

