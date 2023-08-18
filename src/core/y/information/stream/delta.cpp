

#include "y/information/stream/delta.hpp"



namespace Yttrium
{
    namespace Information
    {
        Delta:: Common:: Common() :
        StreamCipher(),
        last(0)
        {
        }

        Delta:: Common:: ~Common() noexcept
        {

        }

        void Delta:: Common:: restart() noexcept
        {
            last = 0;
        }
    }

}

namespace Yttrium
{
    namespace Information
    {

        Delta:: Encoder:: Encoder() :
        Common()
        {

        }

        Delta:: Encoder:: ~Encoder() noexcept
        {
        }

        uint8_t Delta:: Encoder:: operator()(const uint8_t x) noexcept
        {
            const uint8_t res = x - last;
            last = x;
            return res;
        }

    }

}

namespace Yttrium
{
    namespace Information
    {

        Delta:: Decoder:: Decoder() :
        Common()
        {
        }

        Delta:: Decoder:: ~Decoder() noexcept
        {
        }

        uint8_t Delta:: Decoder:: operator()(const uint8_t x) noexcept
        {
            const uint8_t res = x + last;
            last = res;
            return res;
        }




    }

}
