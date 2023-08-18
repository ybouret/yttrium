
#include "y/information/stream/move-to-front.hpp"
#include "y/data/list.hpp"
#include "y/object.hpp"
#include "y/memory/allocator/dyadic.hpp"

namespace Yttrium
{
    namespace Information
    {

        namespace
        {
            struct MTF
            {
                MTF    *next;
                MTF    *prev;
                uint8_t code;
                void   *priv;
            };
        }

        class MoveToFront:: Code : public Object
        {
        public:
            explicit Code() noexcept {}
            virtual ~Code() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
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
            std::cerr << "sizeof(Code) = " << sizeof(Code) << std::endl;
            std::cerr << "sizeof(MTF)  = " << sizeof(MTF) << std::endl;

        }

        MoveToFront:: Encoder:: ~Encoder() noexcept
        {
        }

        uint8_t MoveToFront:: Encoder:: operator()(const uint8_t x) noexcept
        {
            assert(0!=code);
            return x;
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
            return x;
        }


    }

}
