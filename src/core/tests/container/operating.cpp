
#include "y/container/operating.hpp"
#include "y/utest/run.hpp"
#include "y/apex/natural.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/container/iterator/display-all.hpp"
#include "y/container/light-array.hpp"
#include "y/random/fill.hpp"
#include <cstring>

using namespace Yttrium;



Y_UTEST(container_operating)
{
    Random::Rand ran;

    void *wksp1[100];
    void *wksp2[100];
    void *wksp3[100];
    void *wksp4[100];

    Random::Fill::Block(wksp1,sizeof(wksp1),ran,1);
    Random::Fill::Block(wksp2,sizeof(wksp2),ran,1);
    Random::Fill::Block(wksp3,sizeof(wksp3),ran,1);
    Random::Fill::Block(wksp4,sizeof(wksp4),ran,1);


    const size_t    n = 10;
    Operating<apn>  op1(wksp1,n);
    LightArray<apn> a1( op1(), op1.blocks() );
    for(size_t i=a1.size();i>0;--i)
    {
        a1[i] = apn(10,ran);
    }
    std::cerr << a1 << std::endl;
    Iterating::DisplayAll::Of(a1);

    Operating<apn>  op2(wksp2,op1);
    LightArray<apn> a2( op2(), op2.blocks() );

    std::cerr << a2 << std::endl;
    Iterating::DisplayAll::Of(a2);

    Operating<uint64_t>  op3(wksp3,n);
    LightArray<uint64_t> a3( op3(), op3.blocks() );
    std::cerr << a3 << std::endl;
    for(size_t i=a3.size();i>0;--i)
    {
        a3[i] = ran.to<uint64_t>(12);
    }
    std::cerr << a3 << std::endl;


}
Y_UDONE()
