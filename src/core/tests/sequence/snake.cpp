#include "y/sequence/snake.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(sequence_snake)
{
    const size_t length = 4;
    Snake<int> snake(length);

    std::cerr << snake << std::endl;

    for(int i=1;i<=10;++i)
    {
        snake << i;
        std::cerr << snake << std::endl;
    }

    for(int i=1;i<=10;++i)
    {
        snake >> i;
        std::cerr << snake << std::endl;
    }

    snake.free();
    std::cerr << snake << std::endl;
    Y_CHECK(snake.capacity()==length);


}
Y_UDONE()

