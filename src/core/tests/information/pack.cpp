
#include "y/utest/run.hpp"


namespace Yttrium
{

    namespace Information
    {
     
        class Alphabet
        {
        public:
            static const unsigned Bytes = 256;
            static const unsigned Ctrls = 2;
            static const uint16_t Codes = Bytes+Ctrls;
            static const uint16_t EOS   = 256;
            static const uint16_t NYT   = 257;

            


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
