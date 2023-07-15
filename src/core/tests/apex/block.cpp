

#include "y/apex/m/block.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"

using namespace Yttrium;


namespace
{
    template <typename BLOCK>
    static inline void display(const BLOCK &block)
    {
        std::cerr << "Block" << std::setw(2) << BLOCK::WordBytes << " : words=" << block.words;
        std::cerr << " in #" << block.bytes << " bytes" << std::endl;
    }
}

Y_UTEST(apex_block)
{

    Y_SIZEOF(Apex::Block<uint8_t>);  Y_USHOW(Apex::Block<uint8_t>::MaxWords);
    Y_SIZEOF(Apex::Block<uint16_t>); Y_USHOW(Apex::Block<uint16_t>::MaxWords);
    Y_SIZEOF(Apex::Block<uint32_t>); Y_USHOW(Apex::Block<uint32_t>::MaxWords);
    Y_SIZEOF(Apex::Block<uint64_t>); Y_USHOW(Apex::Block<uint64_t>::MaxWords);



    Apex::Block<uint8_t>  b8(4);  display(b8);
    Apex::Block<uint16_t> b16(4); display(b16);
    Apex::Block<uint32_t> b32(4); display(b32);
    Apex::Block<uint64_t> b64(4); display(b64);


}
Y_UDONE()
