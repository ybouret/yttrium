

#include "y/apex/m/block.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"
#include "y/random/fill.hpp"

using namespace Yttrium;


namespace
{
    template <typename BLOCK>
    static inline void display(Random::Bits &ran)
    {
        std::cerr << std::endl;
        Y_USHOW(BLOCK::WordBytes);
        Y_USHOW(BLOCK::WordShift);
        BLOCK block(0);
        Y_USHOW(block.bytes);
        Y_USHOW(block.shift);
        Y_USHOW(block.words);

        Random::Fill::Block(block.entry,block.bytes,ran,1,255);

        std::cerr << "\tcopy" << std::endl;
        {
            const BLOCK bcopy(block);
            Y_USHOW(bcopy.bytes);
            Y_USHOW(bcopy.shift);
            Y_USHOW(bcopy.words);
            for(size_t i=0;i<block.words;++i)
            {
                Y_ASSERT(bcopy.entry[i]==block.entry[i]);
            }
        }

        std::cerr << "\tcopy+" << std::endl;
        {
            const BLOCK bcopy(block,Apex::IncreaseSize);
            Y_USHOW(bcopy.bytes);
            Y_USHOW(bcopy.shift);
            Y_USHOW(bcopy.words);
            for(size_t i=0;i<block.words;++i)
            {
                Y_ASSERT(bcopy.entry[i]==block.entry[i]);
            }
        }

    }
}

Y_UTEST(apex_block)
{

    Random::Rand ran;
    display< Apex::Block<uint8_t>  >(ran);
    display< Apex::Block<uint16_t> >(ran);
    display< Apex::Block<uint32_t> >(ran);
    display< Apex::Block<uint64_t> >(ran);

}
Y_UDONE()
