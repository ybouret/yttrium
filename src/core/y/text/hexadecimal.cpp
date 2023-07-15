
#include "y/text/hexadecimal.hpp"
#include <cstring>
#include <iostream>

namespace Yttrium
{

    const char  Hexadecimal:: Text[256][4] =
    {
        "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0a", "0b", "0c", "0d", "0e", "0f",
        "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1a", "1b", "1c", "1d", "1e", "1f",
        "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2a", "2b", "2c", "2d", "2e", "2f",
        "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3a", "3b", "3c", "3d", "3e", "3f",
        "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "4a", "4b", "4c", "4d", "4e", "4f",
        "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "5a", "5b", "5c", "5d", "5e", "5f",
        "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6a", "6b", "6c", "6d", "6e", "6f",
        "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7a", "7b", "7c", "7d", "7e", "7f",
        "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "8a", "8b", "8c", "8d", "8e", "8f",
        "90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9a", "9b", "9c", "9d", "9e", "9f",
        "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "a8", "a9", "aa", "ab", "ac", "ad", "ae", "af",
        "b0", "b1", "b2", "b3", "b4", "b5", "b6", "b7", "b8", "b9", "ba", "bb", "bc", "bd", "be", "bf",
        "c0", "c1", "c2", "c3", "c4", "c5", "c6", "c7", "c8", "c9", "ca", "cb", "cc", "cd", "ce", "cf",
        "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7", "d8", "d9", "da", "db", "dc", "dd", "de", "df",
        "e0", "e1", "e2", "e3", "e4", "e5", "e6", "e7", "e8", "e9", "ea", "eb", "ec", "ed", "ee", "ef",
        "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "fa", "fb", "fc", "fd", "fe", "ff"
    };

    const char Hexadecimal:: Lower[16] =
    {
        '0', '1', '2', '3',
        '4', '5', '6', '7',
        '8', '9', 'a', 'b',
        'c', 'd', 'e', 'f'
    };

    const char Hexadecimal:: Upper[16] =
    {
        '0', '1', '2', '3',
        '4', '5', '6', '7',
        '8', '9', 'A', 'B',
        'C', 'D', 'E', 'F'
    };


    int Hexadecimal:: ToDecimal(const char c) noexcept
    {
        switch(c)
        {
            case '0': return 0;
            case '1': return 1;
            case '2': return 2;
            case '3': return 3;
            case '4': return 4;
            case '5': return 5;
            case '6': return 6;
            case '7': return 7;
            case '8': return 8;
            case '9': return 9;
            case 'a':
            case 'A':
                return 10;
            case 'b':
            case 'B':
                return 11;
            case 'c':
            case 'C':
                return 12;
            case 'd':
            case 'D':
                return 13;
            case 'e':
            case 'E':
                return 14;
            case 'f':
            case 'F':
                return 15;
        }
        return -1;
    }

    Hexadecimal:: Hexadecimal(const Hexadecimal &other) noexcept :
    data()
    {
        memcpy(data,other.data,CharsPerWord);
    }

    Hexadecimal:: ~Hexadecimal() noexcept
    {
        memset(data,0,sizeof(data));
    }


    void Hexadecimal:: setup(const uint64_t   qw,
                             const unsigned   sz,
                             const OutputSize outputSize,
                             const OutputCase outputCase) noexcept
    {
        assert(sz>0);
        memset(data,0,sizeof(data));
        const char *ch = 0;
        switch(outputCase)
        {
            case LowerCase: ch = Lower; break;
            case UpperCase: ch = Upper; break;
        }
        assert(0!=ch);

        switch(outputSize)
        {
            case Default:
                for(unsigned i=0,j=0,shr=(sz-1)<<3;i<sz;++i,shr -= 8)
                {
                    const uint8_t b = static_cast<uint8_t>(qw >> shr);
                    data[j++] = ch[(b>>4)];
                    data[j++] = ch[(b&0xf)];
                }
                break;

            case Compact: {
                const unsigned n = BytesFor(qw); assert(n<=sz);
                if(n<=0)
                {
                    data[0] = ch[0]; return;
                }
                else
                {
                    bool first = true;
                    for(unsigned i=0,j=0,shr=(n-1)<<3;i<sz;++i,shr -= 8)
                    {
                        const uint8_t b = static_cast<uint8_t>(qw >> shr);
                        const uint8_t l = (b>>4);
                        if(first)
                        {
                            if(l>0)
                                data[j++] = ch[l];
                            first = false;
                        }
                        else
                        {
                            data[j++] = ch[l];
                        }
                        data[j++] = ch[(b&0xf)];
                    }
                }
            } break;
        }


    }

    std::ostream & operator<<(std::ostream &os, const Hexadecimal &h)
    {
        os << h.data;
        return os;
    }
}
