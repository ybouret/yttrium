
#include "y/information/stream/arc4.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/text/ops.hpp"

using namespace Yttrium;

namespace {
    struct TestVector
    {
        const char *key;
        const char *text;
        const char *hexa;

        bool pass() const
        {
            Information::ARC4::Encoder enc(key);
            const String               Plain = text;
            String                     Crypt;
            String                     Hexa;
            for(size_t i=1;i<=Plain.size();++i)
            {
                const uint8_t c = enc(Plain[i]);
                Crypt << c;
                Hexa  << Hexadecimal::Text[c];
            }
            String       good = hexa;
            TextOps::ToLower(&good[1], good.size());
            //std::cerr << Hexa << "/" << good << std::endl;
            return Hexa == good;
        }
    };

    static const TestVector table[] =
    {
        { "Key", "Plaintext", "BBF316E8D940AF0AD3" },
        { "Wiki", "pedia", "1021BF0420" },
        { "Secret", "Attack at dawn", "45A01F645FC35B383552544B9BF5" }
    };


}

Y_UTEST(info_arc4)
{
    Information::ARC4::Encoder enc("Key");;

    Y_CHECK(table[0].pass());
    Y_CHECK(table[1].pass());
    Y_CHECK(table[2].pass());



}
Y_UDONE()

