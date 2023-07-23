#include "y/container/operating.hpp"
#include "y/object.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Core
    {
        class Operating:: Code : public Object
        {
        public:
            void *       entry;
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
                assert(0!=entry);
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

            inline virtual ~Code() noexcept { release(); }

            inline void release() noexcept
            {
                assert(0!=entry);
                assert(0!=erase);
                assert(width>0);

                uint8_t *addr = static_cast<uint8_t *>(entry);
                size_t   size = built*width;
                while(size>0)
                {
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

        Operating:: ~Operating() noexcept
        {
            assert(code);
            delete code;
            code=0;
        }
    }
}

