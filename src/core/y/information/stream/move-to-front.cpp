
#include "y/information/stream/move-to-front.hpp"
#include "y/object.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Information
    {


        class MoveToFront:: Code : public Object
        {
        public:
            static const unsigned Shift = iLog2<256>::Value;

            explicit Code() :
            Object(),
            shift(Shift),
            entry( alloc() )
            {
                format();
            }

            virtual ~Code() noexcept
            {
                static Memory::Dyadic &mgr = Memory::Dyadic::Location();
                mgr.releaseBlock( *(void**)&entry,shift);
            }

            inline void format() noexcept
            {
                for(unsigned i=0;i<256;++i)
                {
                    entry[i] = uint8_t(i);
                }
            }

            uint8_t *lookFor(const uint8_t x) const noexcept
            {
                uint8_t *p = entry;

            CHECK:
                if(*p == x ) return p;
                ++p;
                goto CHECK;
            }

            inline uint8_t encode(const uint8_t x) noexcept
            {
                uint8_t      *p = lookFor(x);
                const uint8_t n = static_cast<uint8_t>(p-entry);
                memmove(entry+1,entry,n);
                entry[0] = x;
                return n;
            }

            inline uint8_t decode(const uint8_t n) noexcept
            {
                uint8_t *     p = entry+n;
                const uint8_t x = *p;
                memmove(entry+1,entry,n);
                entry[0] = x;
                return x;
            }


            unsigned  shift;
            uint8_t  *entry;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
            inline uint8_t *alloc() {
                static Memory::Dyadic &mgr = Memory::Dyadic::Instance();
                return static_cast<uint8_t*>(mgr.acquireBlock(shift));
            }


        };
    }

}

namespace Yttrium
{
    namespace Information
    {
        MoveToFront:: Common:: Common() :
        StreamCipher(),
        code(new Code())
        {
        }

        MoveToFront:: Common:: ~Common() noexcept
        {
            assert(0!=code);
            delete code;
            code = 0;
        }

        void MoveToFront:: Common:: restart() noexcept
        {
            assert(0!=code);
            code->format();
        }
    }

}

namespace Yttrium
{
    namespace Information
    {

        MoveToFront:: Encoder:: Encoder() :
        Common()
        {
            
        }

        MoveToFront:: Encoder:: ~Encoder() noexcept
        {
        }

        uint8_t MoveToFront:: Encoder:: operator()(const uint8_t x) noexcept
        {
            assert(0!=code);
            return code->encode(x);
        }

    }

}

namespace Yttrium
{
    namespace Information
    {

        MoveToFront:: Decoder:: Decoder() :
        Common()
        {
        }

        MoveToFront:: Decoder:: ~Decoder() noexcept
        {
        }

        uint8_t MoveToFront:: Decoder:: operator()(const uint8_t x) noexcept
        {
            assert(0!=code);
            return code->decode(x);
        }




    }

}
