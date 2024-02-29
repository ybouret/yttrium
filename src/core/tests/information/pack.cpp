
#include "y/utest/run.hpp"


namespace Yttrium
{

    namespace Information
    {
     
        class Alphabet
        {
        public:
            static const unsigned Singles  = 256;
            static const unsigned Couples  = 65536;
            static const unsigned Escapes  = 2;
            static const unsigned Codes    = Singles+Couples+Escapes;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Alphabet);
        };

    }

}

using namespace Yttrium;


Y_UTEST(info_pack)
{

}
Y_UDONE()
