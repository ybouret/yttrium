#include "y/sequence/snake.hpp"
#include "y/utest/run.hpp"
#include "y/container/iterator/display-all.hpp"
#include "y/apex/rational.hpp"
#include "y/random/bits.hpp"

using namespace Yttrium;

Y_UTEST(sequence_snake)
{

    {
        const size_t length = 4;
        Snake<int> snake(length);

        std::cerr << snake << std::endl;

        for(int i=1;i<=10;++i)
        {
            snake << i;
            std::cerr << snake << std::endl;
        }

        Iterating::DisplayAll::Of(snake);

        for(int i=1;i<=10;++i)
        {
            snake >> i;
            std::cerr << snake << std::endl;
        }

        snake.free();
        std::cerr << snake << std::endl;
        Y_CHECK(snake.capacity()==length);
    }

    {
        Snake<apq>   snake(3);
        Random::Rand ran;
        for(size_t i=0;i<=10;++i)
        {
            const apq q(16,16,ran);
            snake << q;
            std::cerr << snake << std::endl;
        }
    }


}
Y_UDONE()

