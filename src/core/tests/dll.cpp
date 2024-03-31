#include "y/rtld/dll.hpp"
#include "y/program.hpp"
#include "y/text/hexadecimal.hpp"

using namespace Yttrium;

Y_Program()
{

    if(argc>1)
    {
        DLL dll(argv[1]);
        void *hData = dll.symbol("Data");
        if(hData)
        {
            const int32_t data = *(const int32_t *)hData;
            std::cerr << "0x" << Hexadecimal(data) << std::endl;
        }
        else
        {
            std::cerr << "Data not found!" << std::endl;
        }


    }
}
Y_End()

