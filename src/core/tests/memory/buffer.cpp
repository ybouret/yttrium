


#include "y/memory/buffer/fixed.hpp"
#include "y/memory/buffer/of.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;


Y_UTEST(memory_buffer)
{

    Memory::FixedBuffer<12> fb;
    std::cerr << fb.measure() << std::endl;

    Memory::BufferOf<int>  ib(10);
    

}
Y_UDONE()
