#include "y/type/binder.hpp"

#include "y/utest/run.hpp"
#include "y/string.hpp"


using namespace Yttrium;



Y_UTEST(type_binder)
{

    {
        std::cerr << "NullType" << std::endl;
        Binder<NullType> bnd;
        std::cerr << bnd << std::endl << std::endl;
    }

    {
        std::cerr << "const int/default" << std::endl;
        Binder<TL1(const int)> bnd;
        std::cerr << bnd << std::endl << std::endl;
    }

    {
        std::cerr << "const int/value" << std::endl;
        Binder<TL1(const int)> bnd(-7);
        std::cerr << bnd << std::endl << std::endl;
    }

    {
        std::cerr << "String/default" << std::endl;
        Binder<TL1(String)> bnd;
        std::cerr << bnd << std::endl << std::endl;
    }

    {
        std::cerr << "String/value" << std::endl;
        Binder<TL1(String)> bnd("Hello");
        std::cerr << bnd << std::endl << std::endl;
    }

    {
        std::cerr << "int &" << std::endl;
        int a = 2;
        Binder<TL1(int &)> bnd(a);
        std::cerr << bnd << std::endl << std::endl;
    }

}
Y_UDONE()

