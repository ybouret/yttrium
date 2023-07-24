
#include "y/sequence/vector.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"

using namespace Yttrium;


Y_UTEST(sequence_vector)
{
    Random::Rand ran;
    Vector<float> v;

    for(size_t loop=0;loop<16;++loop)
    {
        for(size_t n=3+ran.leq(17);n>0;--n)
        {
            if(ran.choice())
            {
                v.pushHead( ran.to<float>() );
            }
            else
            {
                v.pushTail( ran.to<float>() );
            }
            std::cerr << v << " " << v.size() << " / " << v.capacity() << std::endl;
        }
        while(v.size()>0)
        {
            if(ran.choice()) v.popHead(); else v.popTail();
            std::cerr << v << std::endl;
        }
        v.release();
    }

}
Y_UDONE()

